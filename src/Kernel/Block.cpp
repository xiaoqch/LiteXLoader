#include "Block.h"
using namespace std;

string Raw_GetBlockName(Block* block)
{
    void* hashedstr = SymCall("?getName@Block@@QEBAAEBVHashedString@@XZ",
        void*, void*)(block);
    auto blockname = ((const char*)hashedstr + 8);
    
    return string(blockname);
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