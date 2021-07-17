#include "Block.h"
#include "SymbolHelper.h"
using namespace std;

string Raw_GetBlockName(Block* block)
{
    HashedString* hashedstr = SymCall("?getName@Block@@QEBAAEBVHashedString@@XZ",
        HashedString*, void*)(block);
    
    return hashedstr->getString();
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