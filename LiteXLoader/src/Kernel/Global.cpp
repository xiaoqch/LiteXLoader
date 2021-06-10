#include "Global.h"
#include <tuple>
using namespace std;

//声明
class Scoreboard;

//全局变量
Minecraft* mc = nullptr;
Scoreboard* g_scoreboard = nullptr;
CommandRegistry* CmdReg = nullptr;
bool isServerStarted = false;

//命令注册等待队列
std::vector<tuple<string, string, int>> toRegCmdQueue;


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