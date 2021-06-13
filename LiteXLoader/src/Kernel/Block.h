#include "Global.h"
#include <string>

std::string Raw_GetBlockName(Block* block);
std::string Raw_GetBlockFullName(Block* block);
int Raw_GetBlockDimension(BlockSource *bs);

Block* Raw_GetBlockByPos(IntVec4 *pos);
Block* Raw_GetBlockByPos(int x, int y, int z, BlockSource* bs);