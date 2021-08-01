#include "Global.h"
#include "SymbolHelper.h"
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

BlockSource* Raw_GetBlockSourceByDim(int dimid)
{
	auto dim = (int*) SymCall("?getDimension@Level@@UEBAPEAVDimension@@V?$AutomaticID@VDimension@@H@@@Z",
		uintptr_t, void*, int)(mc->getLevel(), dimid);
	return dAccess<BlockSource*>(dim, 96);
}

Block* Raw_GetBlockByPos(BlockPos* bp, BlockSource* bs)
{
	return Raw_GetBlockByPos(bp->x, bp->y, bp->z, bs);
}

Block* Raw_GetBlockByPos(int x, int y, int z, BlockSource* bs)
{
	return SymCall("?getBlock@BlockSource@@QEBAAEBVBlock@@HHH@Z",
		Block*, BlockSource*, int, int, int)(bs, x, y, z);
}

Block* Raw_GetBlockByPos(BlockPos *bp, int dimid)
{
	auto bs = Raw_GetBlockSourceByDim(dimid);
	return Raw_GetBlockByPos(bp->x, bp->y, bp->z, bs);
}

Block* Raw_GetBlockByPos(IntVec4* pos)
{
	auto bs = Raw_GetBlockSourceByDim(pos->dim);
	return Raw_GetBlockByPos(pos->x, pos->y, pos->z, bs);
}

Actor* Raw_GetFishingHookOwner(FishingHook* fh)
{
	return SymCall("?getOwner@FishingHook@@QEAAPEAVActor@@XZ",
		Actor*, FishingHook*)(fh);
}