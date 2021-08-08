#include "Global.h"
#include "Entity.h"
using namespace std;

//声明
class Scoreboard;

//全局变量
bool isServerStarted = false;
bool isCmdRegisterEnabled = false;

Minecraft* mc = nullptr;
Scoreboard* globalScoreBoard = nullptr;
CommandRegistry* CmdReg = nullptr;
DataLoadHelper* gDataHelperPlayer = nullptr;
DataLoadHelper* gDataHelperActor = nullptr;

//初始化
THook(Scoreboard*, "??0ServerScoreboard@@QEAA@VCommandSoftEnumRegistry@@PEAVLevelStorage@@@Z",
	void* _this, void* a2, void* a3)
{
	globalScoreBoard = (Scoreboard*)original(_this, a2, a3);
	return globalScoreBoard;
}

THook(void, "?initAsDedicatedServer@Minecraft@@QEAAXXZ",
	void* self)
{
	original(self);
	mc = (Minecraft*)self;
}

class CompoundTag;
THook(bool, "?load@Actor@@UEAA_NAEBVCompoundTag@@AEAVDataLoadHelper@@@Z",
	Actor* _this, CompoundTag* tag, DataLoadHelper* data)
{
	if (!gDataHelperPlayer)
	{
		if (Raw_IsPlayer(_this))
			gDataHelperPlayer = data;
	}
	if (!gDataHelperActor)
	{
		if(!Raw_IsPlayer(_this))
			gDataHelperActor = data;
	}
	return original(_this, tag, data);
}