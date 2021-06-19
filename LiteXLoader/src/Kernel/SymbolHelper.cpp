#include "Global.h"
using namespace std;


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
	extern Minecraft* mc;
	auto dim = SymCall("?getDimension@Level@@UEBAPEAVDimension@@V?$AutomaticID@VDimension@@H@@@Z",
		uintptr_t, void*, int)(mc->getLevel(), pos->dim);
	auto bs = (BlockSource*)(dim + 88);

	return Raw_GetBlockByPos(pos->x, pos->y, pos->z, bs);
}