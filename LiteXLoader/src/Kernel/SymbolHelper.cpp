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

//命令注册等待队列
std::vector<tuple<string, string, int>> toRegCmdQueue;

//辅助函数
BlockSource* Raw_GetBlockSourceByActor(Actor* actor)
{
	return dAccess<BlockSource*>(actor, 872);
}

//////////////////////// Helper ////////////////////////
class AutomaticID<class Dimension, int> GetDimensionId(BlockSource* bs)
{
	class AutomaticID<class Dimension, int>(BlockSource:: * rv)() const;
	*((void**)&rv) = dlsym("?getDimensionId@BlockSource@@QEBA?AV?$AutomaticID@VDimension@@H@@XZ");
	return (bs->*rv)();
}
//////////////////////// Helper ////////////////////////

int Raw_GetBlockDimension(BlockSource* bs)
{
	return GetDimensionId(bs);
}

Block* Raw_GetBlockByPos(int x, int y, int z, BlockSource* bs)
{
	return SymCall("?getBlock@BlockSource@@QEBAAEBVBlock@@HHH@Z",
		Block*, BlockSource*, int, int, int)(bs, x, y, z);
}

Block* Raw_GetBlockByPos(IntVec4* pos)
{
	auto dim = SymCall("?getDimension@Level@@UEBAPEAVDimension@@V?$AutomaticID@VDimension@@H@@@Z",
		uintptr_t, void*, int)(mc->getLevel(), pos->dim);
	auto bs = (BlockSource*)(dim + 88);

	return Raw_GetBlockByPos(pos->x, pos->y, pos->z, bs);
}