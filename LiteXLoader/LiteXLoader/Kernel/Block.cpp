#include "Global.h"
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

Block* Raw_NewBlockFromName(string name)
{
    string blockName = "?m" + name + "@VanillaBlocks@@3PEBVBlock@@EB";
    return *(Block**)dlsym_real(blockName.c_str());
}

bool Raw_SetBlockByBlock(IntVec4 pos, Block* block)
{
    BlockSource* bs = Raw_GetBlockSourceByDim(pos.dim);

    return SymCall("?setBlock@BlockSource@@QEAA_NAEBVBlockPos@@AEBVBlock@@HPEBUActorBlockSyncMessage@@@Z",
        bool, BlockSource*, BlockPos , Block*, int, void*)
        (bs, { pos.x, pos.y, pos.z }, block, 0, nullptr);
}

bool Raw_SetBlockByName(IntVec4 pos, const string& name)
{
    Block* newBlock = Raw_NewBlockFromName(name);
    if (!newBlock)
        return false;
    return Raw_SetBlockByBlock(pos, newBlock);
}

bool Raw_SpawnParticle(IntVec4 pos, const string& type)
{
    string name = type;
    Level* level = mc->getLevel();
    Dimension *dim = SymCall("?getDimension@Level@@UEBAPEAVDimension@@V?$AutomaticID@VDimension@@H@@@Z",
        Dimension*, void*, AutomaticID<Dimension, int>)(level, AutomaticID<Dimension, int>(pos.dim));

    SymCall("?spawnParticleEffect@Level@@UEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBVVec3@@PEAVDimension@@@Z",
        void, Level*, string&, const Vec3&, void*)
        (level, name, { (float)pos.x,(float)pos.y,(float)pos.z }, dim);

    return true;
}