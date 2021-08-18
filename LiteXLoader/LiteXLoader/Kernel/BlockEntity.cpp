#include "Global.h"
#include "BlockEntity.h"
#include "Block.h"
#include "SymbolHelper.h"
using namespace std;

BlockActor* Raw_GetBlockEntity(IntVec4 pos)
{
	BlockPos bp{ pos.x,pos.y,pos.z };
	return SymCall("?getBlockEntity@BlockSource@@QEAAPEAVBlockActor@@AEBVBlockPos@@@Z",
		BlockActor*, BlockSource*, BlockPos*)(Raw_GetBlockSourceByDim(pos.dim), &bp);
}

bool Raw_HasBlockEntity(Block* bl)
{
	return SymCall("?hasBlockEntity@Block@@QEBA_NXZ", bool, Block*)(bl);
}

int Raw_GetBlockEntityType(Block* bl)
{
	return dAccess<unsigned int>(bl, 192);		//IDA  Block::getBlockEntityType
}

unsigned int Raw_GetBlockEntityType(BlockActor* ba)
{
	return *SymCall("?getType@BlockActor@@QEBAAEBW4BlockActorType@@XZ", unsigned int*, BlockActor*)(ba);
}

IntVec4 Raw_GetBlockEntityPos(BlockActor* ba)
{
	BlockPos bp = ba->getPosition();
	return { bp.x,bp.y,bp.z,Raw_GetBlockDimension(Raw_GetBlockSourceByActor((Actor*)ba)) };		//####### May have problem
}