#include "pch.h"
using namespace std;

//声明
class Scoreboard;

//全局变量
bool isServerStarted = false;
bool isCmdRegisterEnabled = false;

Minecraft* mc = nullptr;
Scoreboard* g_scoreboard = nullptr;
CommandRegistry* CmdReg = nullptr;


//初始化
THook(Scoreboard*, "??0ServerScoreboard@@QEAA@VCommandSoftEnumRegistry@@PEAVLevelStorage@@@Z",
	void* _this, void* a2, void* a3)
{
	g_scoreboard = (Scoreboard*)original(_this, a2, a3);
	return g_scoreboard;
}

THook(void, "?initAsDedicatedServer@Minecraft@@QEAAXXZ",
	void* self)
{
	original(self);
	mc = (Minecraft*)self;
}