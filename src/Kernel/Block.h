#include "Global.h"
#include <string>

std::string Raw_GetBlockName(Block* block);
int Raw_GetBlockDimension(BlockSource *bs);

Block* Raw_GetBlockByPos(IntVec4 *pos);