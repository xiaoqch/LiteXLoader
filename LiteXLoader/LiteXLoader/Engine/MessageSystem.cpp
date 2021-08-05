#include "MessageSystem.h"
#include <API/APIHelp.h>
#include <Kernel/Utils.h>
#include "GlobalShareData.h"
#include <process.h>
#include <exception>
using namespace std;

//////////////////// 消息处理注册 ////////////////////

void ProcessModuleMessage(ModuleMessage &msg)
{
    switch (msg.getType())
    {
    case ModuleMessage::MessageType::RemoteCall:
        break;
    case ModuleMessage::MessageType::RemoteCallReturn:
        break;
    case ModuleMessage::MessageType::LxlCommand:
        break;
    default:
        break;
    }
}


//////////////////// Class ////////////////////

ModuleMessage::ModuleMessage(ModuleMessage::MessageType type, void* data)
{
    packData = new MessagePackData;

    packData->id = getNextMessageId();

    this->type = type;
    this->data = data;
}

ModuleMessage::ModuleMessage(int msgId, MessageType type, void* data)
{
    packData = new MessagePackData;

    packData->id = msgId;
    packData->senderThread = globalShareData->messageSystemList[LXL_MODULE_TYPE].threadId;
    this->type = type;
    this->data = data;
}

ModuleMessage::ModuleMessage(UINT type, LPARAM lp, WPARAM wp)
{
    this->type = (ModuleMessage::MessageType)type;
    packData = (MessagePackData*)lp;
    data = (void*)wp;
}

LPARAM ModuleMessage::getLParam()
{
    return (LPARAM)packData;
}

WPARAM ModuleMessage::getWParam()
{
    return (WPARAM)data;
}

int ModuleMessage::getNextMessageId()
{
    return ++globalShareData->messageSystemNextId;
}

void ModuleMessage::destroy()
{
    delete packData;
}

int ModuleMessage::broadcast()
{
    int sent = 0;

    for (auto& mod : globalShareData->messageSystemList)
    {
        if (mod.first != LXL_MODULE_TYPE)
            if (PostThreadMessage(mod.second.threadId, (UINT)getType(), getWParam(), getLParam()))
                ++sent;
    }
    return sent;
}

int ModuleMessage::broadcastAll()
{
    int sent = 0;

    for (auto& mod : globalShareData->messageSystemList)
    {
        if (PostThreadMessage(mod.second.threadId, (UINT)getType(), getWParam(), getLParam()))
            ++sent;
    }
    return sent;
}

bool ModuleMessage::sendTo(string toModuleType)
{
    try
    {
        unsigned int threadId = globalShareData->messageSystemList.at(toModuleType).threadId;
        PostThreadMessage(threadId, (UINT)getType(), getWParam(), getLParam());
    }
    catch (const std::out_of_range& e)
    {
        return false;
    }
}

bool ModuleMessage::sendBack(ModuleMessage& msg)
{
    return PostThreadMessage(packData->senderThread, (UINT)msg.getType(), msg.getWParam(), msg.getLParam());
}

bool ModuleMessage::waitForMessage(int messageId, int maxWaitTime)
{
    ModuleMessage::syncWaitList[messageId] = true;

    auto fromTime = GetCurrentTimeStampMS();
    bool isSuccess = false;
    while (maxWaitTime < 0 ? true : GetCurrentTimeStampMS() - fromTime <= maxWaitTime)
    {
        Sleep(LXL_MESSAGE_SYSTEM_WAIT_CHECK_INTERVAL);
        if (!ModuleMessage::syncWaitList[messageId])
        {
            isSuccess = true;
            break;
        }
    }
    ModuleMessage::syncWaitList.erase(messageId);
    return isSuccess;
}


//////////////////// APIs ////////////////////

unsigned __stdcall ModuleMessageLoop(void* pParam)
{
    BOOL bRet;
    MSG message;

    while ((bRet = GetMessage(&message, NULL, (UINT)ModuleMessage::MessageType::LXL_MODULE_MSG_BEGIN, (UINT)ModuleMessage::MessageType::LXL_MODULE_MSG_END)) != 0)
    {
        if (bRet == -1)
        {
            FATAL(string("ERROR Ocurred in MessageSystem! Error Code: ") + std::to_string(GetLastError()));
            return -1;
        }
        else
        {
            ModuleMessage msg(message.message, message.lParam, message.wParam);
            ProcessModuleMessage(msg);
            try
            {
                ModuleMessage::syncWaitList.at(msg.getId()) = false;
            }
            catch (...) { ; }

            msg.destroy();
        }
    }
    return 0;
}

bool InitMessageSystem()
{
    unsigned threadId;

    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ModuleMessageLoop, NULL, 0, &threadId);
    if (hThread == NULL)
    {
        ERROR("模块消息循环系统初始化失败");
        return false;
    }
    CloseHandle(hThread);

    (globalShareData->messageSystemList)[LXL_MODULE_TYPE].threadId = threadId;
    return true;
}