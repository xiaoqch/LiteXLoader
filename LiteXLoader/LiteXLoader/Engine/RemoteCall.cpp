#include <API/APIHelp.h>
#include <Engine/RemoteCall.h>
#include <Engine/GlobalShareData.h>
#include <string>
#include <map>
#include <process.h>
#include <Configs.h>
#include <ScriptX/ScriptX.h>

using namespace std;


//消息循环数据
#define LXL_REMOTE_CALL         WM_USER + 15
#define LXL_REMOTE_CALL_RETURN  WM_USER + 16

struct RemoteCallData
{
    unsigned threadFrom;
    string funcName;
    string args;
};

struct RemoteCallReturn
{
    int callId;
    string result;
};

HANDLE hReturnEvent;
RemoteCallReturn *result;
//################# 加入CallId验证

//////////////////// 消息循环 ////////////////////

unsigned __stdcall MessageLoop(void* pParam)
{
    BOOL bRet;
    MSG msg;

    while ((bRet = GetMessage(&msg, NULL, LXL_REMOTE_CALL, LXL_REMOTE_CALL_RETURN)) != 0)
    {
        if (bRet == -1)
        {
            FATAL(string("ERROR Ocurred in MessageLoop! Error Code: ") + to_string(GetLastError()));
            return -1;
        }
        else
        {
            if (msg.message == LXL_REMOTE_CALL)
            {
                RemoteCallData *callData = (RemoteCallData*)msg.wParam;
                RemoteCallReturn* rtn = new RemoteCallReturn;

                try
                {
                    ExportedFuncData* funcData = &(globalShareData->exportedFuncs).at(callData->funcName);
                    EngineScope enter(funcData->engine);

                    Local<Array> args = JsonToValue(callData->args).asArray();
                    vector<Local<Value>> argsList;
                    for (int i = 0; i < args.size(); ++i)
                        argsList.push_back(args.get(i));

                    Local<Value> result = funcData->func.get().call({}, argsList);

                    rtn->result = ValueToJson(result);

                    if (!PostThreadMessage(callData->threadFrom, LXL_REMOTE_CALL_RETURN, (WPARAM)rtn, NULL))
                    {
                        ERROR(string("Fail to post remote call result return! Error Code: ") + to_string(GetLastError()));
                    }
                    ExitEngineScope exit;
                }
                catch (...)
                {
                    ERROR("Error occurred in remote engine!");
                    rtn->result = "null";
                    if (!PostThreadMessage(callData->threadFrom, LXL_REMOTE_CALL_RETURN, (WPARAM)rtn, NULL))
                    {
                        ERROR(string("Fail to post remote call result return! Error Code: ") + to_string(GetLastError()));
                    }
                }
            }
            else if (msg.message == LXL_REMOTE_CALL_RETURN)
            {
                result = (RemoteCallReturn*)msg.wParam;
                SetEvent(hReturnEvent);
            }
        }
    }
    return 0;
}

//////////////////// Remote Call ////////////////////

bool InitRemoteCallSystem()
{
    unsigned threadId;

    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, MessageLoop, NULL, 0, &threadId);
    if (hThread == NULL)
    {
        ERROR(_TRS("remoteCall.createMessageThread.fail"));
        return false;
    }
    CloseHandle(hThread);
    hReturnEvent = CreateEvent(NULL, FALSE, FALSE, LXL_REMOTE_CALL_EVENT_NAME);
    if (hReturnEvent == NULL)
    {
        ERROR(_TRS("remoteCall.createEventObject.fail"));
        return false;
    }

    (globalShareData->remoteEngineList)[LXL_SCRIPT_LANG_TYPE].threadId = threadId;
    return true;
}

Local<Value> MakeRemoteCall(ExportedFuncData* data, const string& funcName, const string& argsList)
{
    RemoteCallData* callData = new RemoteCallData;
    callData->args = argsList;
    callData->funcName = funcName;
    callData->threadFrom = (globalShareData->remoteEngineList)[LXL_SCRIPT_LANG_TYPE].threadId;

    if (!PostThreadMessage((globalShareData->remoteEngineList)[data->fromEngineType].threadId,
        LXL_REMOTE_CALL, (WPARAM)callData, NULL))
    {
        ERROR(_TRS("remoteCall.postMessage.fail"));
        return Local<Value>();
    }

    DWORD waitRes = WaitForSingleObject(hReturnEvent, LXL_REMOTE_CALL_MAX_WAIT * 1000);
    if (waitRes == WAIT_TIMEOUT)
    {
        ERROR(_TRS("remoteCall.timeout.fail"));
        return Local<Value>();
    }
    else if (waitRes == WAIT_FAILED)
    {
        ERROR(_TRS("remoteCall.waitFail.fail"));
        return Local<Value>();
    }

    Local<Value> res = JsonToValue(result->result);
    delete callData;
    delete result;
    return res;
}

bool LxlExportFunc(ScriptEngine *engine, const Local<Function> &func, const string &exportName)
{
    ExportedFuncData* funcData = &(globalShareData->exportedFuncs)[exportName];
    funcData->engine = engine;
    funcData->func = Global<Function>(func);
    funcData->fromEngineType = LXL_SCRIPT_LANG_TYPE;
    return true;
}

bool LxlRemoveAllExportedFuncs(ScriptEngine* engine)
{
    for (auto exp = globalShareData->exportedFuncs.begin(); exp != globalShareData->exportedFuncs.end(); ++exp)
    {
        if (exp->second.engine == engine)
            globalShareData->exportedFuncs.erase(exp);
    }
    return true;
}


//////////////////// APIs ////////////////////

Local<Value> LxlExport(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2)
    CHECK_ARG_TYPE(args[0], ValueKind::kFunction)
    CHECK_ARG_TYPE(args[1], ValueKind::kString)

    try {
        return Boolean::newBoolean(LxlExportFunc(EngineScope::currentEngine(), args[0].asFunction(), args[1].toStr()));
    }
    CATCH("Fail in LxlExport!")
}

Local<Value> LxlImport(const Arguments &args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)
    if (args.size() >= 2)
        CHECK_ARG_TYPE(args[1], ValueKind::kString);

    string funcName;
    try {
        funcName = args[0].toStr();
        ExportedFuncData* funcData = &(globalShareData->exportedFuncs).at(funcName);

        if (funcData->fromEngineType == LXL_SCRIPT_LANG_TYPE)
        {
            //自身DLL调用
            return Function::newFunction([fromEngine{EngineScope::currentEngine()}, engine{ funcData->engine }, funcName]
                (const Arguments& args) -> Local<Value>
            {
                EngineScope enter(engine);

                vector<Local<Value>> argsList;
                for (int i = 0; i < args.size(); ++i)
                    argsList.push_back(args[i]);

                try
                {
                    string res = ValueToJson(globalShareData->exportedFuncs.at(funcName).func.get().call({}, argsList));
                    ExitEngineScope exit;
                    EngineScope rtnResult(fromEngine);
                    return JsonToValue(res);
                }
                catch (const std::out_of_range& e)
                {
                    ERROR(string("Fail to find imported Function [") + funcName + "] !");
                    return Local<Value>();
                }
                CATCH("Fail in imported func!")
            });
        }
        else
        {
            //远程调用
            return Function::newFunction([curEngine{ EngineScope::currentEngine() }, funcData{ funcData }, funcName{ funcName }]
                (const Arguments& args)->Local<Value>
            {
                EngineScope enter(curEngine);

                Local<Array> argsList = Array::newArray();
                for (int i = 0; i < args.size(); ++i)
                    argsList.add(args[i]);

                Local<Value> res = MakeRemoteCall(funcData, funcName, ValueToJson(argsList));
                if (res.isArray())
                    return res.asArray().get(0);
                else
                    return res;
            });
        }
        return Boolean::newBoolean(true);
    }
    catch (const std::out_of_range& e)
    {
        ERROR(string("Fail to import! Function [") + funcName + "] has not been exported!");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in LxlImport!")
}