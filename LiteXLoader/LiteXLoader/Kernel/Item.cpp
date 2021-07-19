#include "pch.h"
#include "Item.h"
#include <string>
#include <vector>
using namespace std;

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

bool Raw_SetLore(ItemStack* item, vector<string> lores)
{
    if (Raw_IsNull(item))
        return false;

    SymCall("?setCustomLore@ItemStackBase@@QEAAXAEBV?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@@Z",
		void, void*, vector<string>)(item, lores);
    return true;
}