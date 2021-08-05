#pragma once
#include <Kernel/Global.h>
#include <ScriptX/ScriptX.h>
#include <string>
#include <map>

//////////////////// Class ////////////////////

class ModuleMessage
{
public:
	enum class MessageType :UINT
	{
		LXL_MODULE_MSG_BEGIN = WM_USER + 15,
		RemoteCall, RemoteCallReturn,
		RemoteRequire, RemoteRequireReturn,
		LxlCommand,
		CallbackIntoTick,
		LXL_MODULE_MSG_END
	};
	struct MessagePackData
	{
		int id;
		unsigned senderThread;
	};

	static int getNextMessageId();
	static std::unordered_map<int, bool> syncWaitList;

private:
	MessageType type;
	MessagePackData *packData;
	void* data;

	LPARAM getLParam();
	WPARAM getWParam();

public:
	ModuleMessage(MessageType type, void* data);
	ModuleMessage(int msgId, MessageType type, void* data);
	ModuleMessage(UINT type, LPARAM lp, WPARAM wp);
	void destroy();

	unsigned getId() { return packData->id; }
	MessageType getType() { return type; }

	int broadcast();
	int broadcastAll();
	bool sendTo(std::string toModuleType);
	bool sendBack(ModuleMessage& msg);
	static bool waitForMessage(int messageId, int maxWaitTime = -1);
};