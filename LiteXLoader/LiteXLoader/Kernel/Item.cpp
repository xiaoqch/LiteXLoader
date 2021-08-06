#include "Global.h"
#include "Item.h"
#include "NBT.h"
#include "SymbolHelper.h"
#include <string>
#include <vector>
using namespace std;

ItemStack* Raw_NewItem(Tag* tag)
{
    try
    {
        char a[272];
        ItemStack* item = SymCall("??0ItemStack@@QEAA@XZ", ItemStack*, ItemStack*)((ItemStack*)a);
        tag->setItem(item);

        return item;
    }
    catch (...)
    {
        return nullptr;
    }
}

class Spawner;
Actor* Raw_SpawnItemByItemStack(ItemStack* item, const FloatVec4& pos)
{
    try
    {
        Spawner* sp = SymCall("?getSpawner@Level@@UEBAAEAVSpawner@@XZ", Spawner*, Level*)(mc->getLevel());

        Vec3 vec{ pos.x,pos.y,pos.z };
        Actor* ac = SymCall("?spawnItem@Spawner@@QEAAPEAVItemActor@@AEAVBlockSource@@AEBVItemStack@@PEAVActor@@AEBVVec3@@H@Z",
            Actor*, Spawner * _this, BlockSource*, ItemStack*, Actor*, Vec3*, int)
            (sp, Raw_GetBlockSourceByDim(pos.dim), item, nullptr, &vec, 0);

        return ac;
    }
    catch (...)
    {
        return nullptr;
    }
}

string Raw_GetItemName(ItemStack* item)
{
    if (Raw_IsNull(item))
        return "";
    return item->getName();
}

string Raw_GetCustomName(ItemStack* item)
{
    if (Raw_IsNull(item))
        return "";
    return item->getCustomName();
}

std::string Raw_GetItemTypeName(ItemStack* item)
{
    if (Raw_IsNull(item))
        return "";
    string fullName;
    SymCall("?getSerializedName@Item@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
		void, Item const*, string*)(item->getItem(), &fullName);
    return fullName; 
}

int Raw_GetItemId(ItemStack* item)
{
    return item->getId();
}

int Raw_GetItemAux(ItemStack* item)
{
    if (Raw_IsNull(item))
        return 0;
    return item->getAuxValue();
}

int Raw_GetCount(ItemStack* item)
{
    if (Raw_IsNull(item))
        return 0;
    return offItemStack::getCount(item);
}

bool Raw_IsNull(ItemStack* item)
{
    return item->isNull();
}

bool Raw_SetNull(ItemStack* item)
{
    SymCall("?setNull@ItemStack@@UEAAXXZ", void, ItemStack*)(item);
    return true;
}

bool Raw_SetLore(ItemStack* item, vector<string> lores)
{
    if (Raw_IsNull(item))
        return false;

    SymCall("?setCustomLore@ItemStackBase@@QEAAXAEBV?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@@Z",
		void, void*, vector<string>)(item, lores);
    return true;
}