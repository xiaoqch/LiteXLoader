#include "Global.h"

//声明
class Scoreboard;

//全局变量
Minecraft* mc;
Scoreboard* g_scoreboard;


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