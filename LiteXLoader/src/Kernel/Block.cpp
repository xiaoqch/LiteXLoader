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
    BlockLegacy* blockLegacy = offBlock::getLegacyBlock(block);
    return offBlockLegacy::getFullName(blockLegacy);
}

int Raw_GetBlockId(Block* block)
{
    BlockLegacy* blockLegacy = offBlock::getLegacyBlock(block);
    return blockLegacy->getBlockItemId();
}