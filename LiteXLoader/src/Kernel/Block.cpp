#include "Block.h"
#include "SymbolHelper.h"
using namespace std;

string Raw_GetBlockName(Block* block)
{
    void* hashedstr = SymCall("?getName@Block@@QEBAAEBVHashedString@@XZ",
        void*, void*)(block);
    auto blockname = ((const char*)hashedstr + 8);
    
    return string(blockname);
}

std::string Raw_GetBlockFullName(Block* block)
{
    return offBlock::getFullName(block);
}

//GetBlockDimension_Helper
class AutomaticID<class Dimension, int> GetDimensionId(BlockSource *bs)
{
    class AutomaticID<class Dimension, int> (BlockSource::*rv)() const;
    *((void **)&rv) = dlsym("?getDimensionId@BlockSource@@QEBA?AV?$AutomaticID@VDimension@@H@@XZ");
    return (bs->*rv)();
}

int Raw_GetBlockDimension(BlockSource *bs)
{
    return GetDimensionId(bs);
}

Block* Raw_GetBlockByPos(IntVec4 *pos)
{
	auto dim = SymCall("?getDimension@Level@@UEBAPEAVDimension@@V?$AutomaticID@VDimension@@H@@@Z",
	      uintptr_t, void*, int)(mc->getLevel(), pos->dim);
	auto bs = (BlockSource*)(dim + 88);

    return Raw_GetBlockByPos(pos->x, pos->y, pos->z , bs);
}

Block* Raw_GetBlockByPos(int x, int y, int z, BlockSource* bs)
{
    return SymCall("?getBlock@BlockSource@@QEBAAEBVBlock@@HHH@Z",
        Block*, BlockSource*, int, int, int)(bs, x, y, z);
}