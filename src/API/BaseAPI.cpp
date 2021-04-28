#include "BaseAPI.h"
#include "APIhelp.h"
#include "PlayerAPI.h"
#include "ItemAPI.h"
#include "BlockAPI.h"
#include "EntityAPI.h"
#include <windows.h>
#include <chrono>
#include <map>
#include <memory>
using namespace script;


//////////////////// Hook ////////////////////

void Hook_RegisterCmd(const string &cmd, const string &describe, int cmdLevel)
{
///////////////////////////// May Have Better Here ///////////////////////////// 
    char *desc = new (std::nothrow) char[describe.size()+1];
    if(!desc)
        return;
    describe.copy(desc, std::string::npos);

    SymCall("?registerCommand@CommandRegistry@@QEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBDW4CommandPermissionLevel@@UCommandFlag@@3@Z",
        void, void*, std::string const&, char const*, char, char, char)
        (CmdRegGlobal, cmd, desc, cmdLevel, 0, 0x40);
}


//////////////////// General APIs ////////////////////

Local<Value> GetName(const Arguments& args)
{ 
    CHECK_ARGS_COUNT(args,1)

    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return String::newString(player->getNameTag());
        
        Actor *entity = ExtractEntity(args[0]);
        if(entity)
            return String::newString(entity->getNameTag());

        Block *block = ExtractBlock(args[0]);
        if(block)
        {
            void* hashedstr = SymCall("?getName@Block@@QEBAAEBVHashedString@@XZ",
                void*, void*)(block);
            auto blockname = ((const char*)hashedstr + 8);
            
            return String::newString(string(blockname));
        }

///////////////////////////// Have better API /////////////////////////////
        ItemStack *item = ExtractItem(args[0]);
        if(item)
            return String::newString(item->getName());
            
        return Local<Value>(); // Null
    }
    CATCH("Fail in GetName!")
}

Local<Value> GetPos(const Arguments& args)
{ 
    CHECK_ARGS_COUNT(args,1)

    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return NewPos(player->getPos(), WPlayer(*player).getDimID());

        Actor *entity = ExtractEntity(args[0]);
        if(entity)
            return NewPos(entity->getPos(), WActor(*entity).getDimID());

///////////////////////////// FIX HERE /////////////////////////////
        /*Block *block = ExtractBlock(args[0]);
        if(block)
        {
            WBlock wp(*block);
            BlockPos bp = wp.getBlockPosition(); 
            
            return NewPos(bp.x, bp.y ,bp.z, wp.getDimID());
        }*/

///////////////////////////// FIX HERE ///////////////////////////// 
        /*ItemStack *item = ExtractItem(args[0]);
        if(item)
            return NewPos(item->getPos(), item->getDimID());*/
            
        return Local<Value>(); // Null
    }
    CATCH("Fail in GetPos!")
}

Local<Value> Teleport(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        FloatPos *pos = ExtractFloatPos(args[1]);
        if(!pos)
            return Local<Value>();

        Player *player = ExtractPlayer(args[0]);
        if(player)
        {
            WPlayer(*player).teleport({pos->x,pos->y,pos->z},pos->dim);
            return Boolean::newBoolean(true);
        }
        
        Actor *entity = ExtractEntity(args[0]);
        if(entity)
        {
            WActor(*entity).teleport({pos->x,pos->y,pos->z},pos->dim);
            return Boolean::newBoolean(true);
        }

        return Local<Value>();    //Null
    }
    CATCH("Fail in Teleport!")
}

Local<Value> Kill(const Arguments& args)
{
    Player *player = ExtractPlayer(args[0]);
    if(player)
    {
        WPlayer(*player).kill();
        return Boolean::newBoolean(true);
    }
    
    Actor *entity = ExtractEntity(args[0]);
    if(entity)
    {
        SymCall("?kill@Mob@@UEAAXXZ", void, void*)(entity);
        return Boolean::newBoolean(true);
    }

    return Local<Value>();    //Null
}

//////////////////// APIs ////////////////////

Local<Value> RunCmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        return Boolean::newBoolean(liteloader::runcmd(args[0].asString().toString()));
    }
    CATCH("Fail in RunCmd!")
}

Local<Value> RunCmdEx(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        std::pair<bool, string> result = liteloader::runcmdEx(args[0].asString().toString());
        Local<Object> resObj = Object::newObject();
        resObj.set("result",result.first);
        resObj.set("output",result.second);
        return resObj;
    }
    CATCH("Fail in RunCmdEx!")
}

Local<Value> RegisterCmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    if(args.size() >= 3)
        CHECK_ARG_TYPE(args[2],ValueKind::kNumber)

    try{
        int level = 4;
        if(args.size() >= 3 && args[2].getKind() == ValueKind::kNumber)
        {
            int newLevel = args[2].asNumber().toInt32();
            if(newLevel >= 0 && newLevel <= 4)
                level = newLevel;
        }
        Hook_RegisterCmd(args[0].asString().toString(),args[1].asString().toString(),level);

        return Boolean::newBoolean(true);
    }
    CATCH("Fail in RegisterCmd!")
}

Local<Value> Log(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)

    try{
        for(int i = 0; i < args.size(); ++i)
            PrintValue(std::cout,args[i]);
        std::cout << std::endl;
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in Log!")
}

Local<Value> GetTimeStr(const Arguments& args)
{
    try{
        time_t t = time(NULL);
        tm* ts = localtime(&t);
        char buf[24]= {0};
        strftime(buf, 24, "%Y-%m-%d %H:%M:%S", ts);
        return String::newString(buf);
    }
    CATCH("Fail in GetTimeStr!")
}

Local<Value> GetTimeNow(const Arguments& args)
{
    try{
        SYSTEMTIME st;
        GetLocalTime(&st);
        Local<Object> res = Object::newObject();
        res.set("Y",Number::newNumber((int)st.wYear));
        res.set("M",Number::newNumber((int)st.wMonth));
        res.set("D",Number::newNumber((int)st.wDay));
        res.set("h",Number::newNumber((int)st.wHour));
        res.set("m",Number::newNumber((int)st.wMinute));
        res.set("s",Number::newNumber((int)st.wSecond));
        res.set("ms",Number::newNumber((int)st.wMilliseconds));
        return res;
    }
    CATCH("Fail in GetTimeNow!")
}

Local<Value> GetLxlVersion(const Arguments& args)
{
    return String::newString(LXL_VERSION);
}


// Time Task
static unsigned long long nowTimeTaskId = 0;
std::map<int32_t,Global<Function>> funcMap;
void inline RemoveFromMap(int32_t id)
{
    funcMap.erase(id);
}

#define TASK_ID msg.data0
#define CODE_STR msg.ptr0
#define CODE_FUNC msg.data2
#define INTERVAL_WAIT msg.data1

// setTimeout
void FuncTimeout(script::utils::Message& msg)
{
    using namespace script::utils;
    EngineScope enter((ScriptEngine*)(msg.tag));
    funcMap[CODE_FUNC].get().call();
    RemoveFromMap(CODE_FUNC);
}
void CodeTimeout(script::utils::Message& msg)
{
    using namespace script::utils;
    ScriptEngine* engine = (ScriptEngine*)(msg.tag);
    EngineScope enter(engine);
    string *toEval = (string*)CODE_STR;
    engine->eval(*toEval);
    if(CODE_STR != nullptr)
    {
        delete CODE_STR;
        CODE_STR = nullptr;
    }
}

Local<Value> SetTimeout(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)

    using namespace script::utils;
    try{
        bool isFunc = args[0].getKind()==ValueKind::kFunction;
        Message msg(isFunc ? FuncTimeout : CodeTimeout, nullptr);
        msg.tag = EngineScope::currentEngine();
        if(isFunc)
        {
            CODE_STR = nullptr;
            funcMap.insert_or_assign(nowTimeTaskId,args[0].asFunction());
            CODE_FUNC = nowTimeTaskId; 
        }
        else
            CODE_STR = new string(args[0].asString().toString());
        int wait = args[1].asNumber().toInt32();
        if(wait < 0)
            wait = 0;
        TASK_ID = nowTimeTaskId;
        globalEngine->messageQueue()->postMessage(msg,std::chrono::milliseconds(wait));
        return Number::newNumber((int64_t)nowTimeTaskId++);
    }
    CATCH("Fail in SetTimeout!")
}

// setInterval
void FuncInterval(script::utils::Message& msg)
{
    using namespace script::utils;
    EngineScope enter((ScriptEngine*)(msg.tag));
    funcMap[CODE_FUNC].get().call();
    globalEngine->messageQueue()->postMessage(msg,std::chrono::milliseconds(INTERVAL_WAIT));
}
void CodeInterval(script::utils::Message& msg)
{
    using namespace script::utils;
    ScriptEngine* engine = (ScriptEngine*)(msg.tag);
    EngineScope enter(engine);
    string *toEval = (string*)CODE_STR;
    engine->eval(*toEval);
    globalEngine->messageQueue()->postMessage(msg,std::chrono::milliseconds(INTERVAL_WAIT));
}

Local<Value> SetInterval(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)

    using namespace script::utils;
    try{
        bool isFunc = args[0].getKind()==ValueKind::kFunction;
        Message msg(isFunc ? FuncInterval : CodeInterval, nullptr);
        msg.tag = EngineScope::currentEngine();
        if(isFunc)
        {
            CODE_STR = nullptr;
            funcMap.insert_or_assign(nowTimeTaskId,args[0].asFunction());
            CODE_FUNC = nowTimeTaskId; 
        }
        else
            CODE_STR = new string(args[0].asString().toString());
        int wait = args[1].asNumber().toInt32();
        if(wait < 0)
            wait = 0;
        TASK_ID = nowTimeTaskId;
        INTERVAL_WAIT = wait;
        globalEngine->messageQueue()->postMessage(msg,std::chrono::milliseconds(wait));
        return Number::newNumber((int64_t)nowTimeTaskId++);
    }
    CATCH("Fail in SetInterval!")
}

// ClearInterval
Local<Value> ClearInterval(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kNumber)

    try{
        using namespace script::utils;
        bool res = globalEngine->messageQueue()->removeMessageIf(
            [&args](Message& msg) -> MessageQueue::RemoveMessagePredReturnType
            {
                if(TASK_ID == args[0].asNumber().toInt64())   //IntervalID
                {
                    if(CODE_STR != nullptr)
                    {
                        delete CODE_STR;
                        CODE_STR = nullptr;
                    }
                    else
                        RemoveFromMap(CODE_FUNC);
                    return MessageQueue::RemoveMessagePredReturnType::kRemove;
                }
                else
                    return MessageQueue::RemoveMessagePredReturnType::kDontRemove;
            }
        );
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in ClearInterval!")
}