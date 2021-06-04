#include "Item.h"
#include <string>
#include <vector>
using namespace std;

string Raw_GetItemName(ItemStack* item)
{
    ///////////////////////////// May Have better API /////////////////////////////
    return item->getName();
}

string Raw_GetCustomName(ItemStack* item)
{
    return item->getCustomName();
}

std::string Raw_GetItemFullName(ItemStack* item)
{
    string fullName;
    SymCall("?getSerializedName@Item@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
		void, Item const*, string*)(item->getItem(), &fullName);
    return fullName; 
}

int Raw_GetItemAux(ItemStack* item)
{
    return item->getAuxValue();
}

int Raw_GetCount(ItemStack* item)
{
    //############## 数量不对 ##############
    return WItem(*item).getCount(); 
}

bool Raw_IsNull(ItemStack* item)
{
    return item->isNull();
}

bool Raw_SetLore(ItemStack* item, vector<string> lores)
{
    //############## 不工作？ ##############
    SymCall("?setCustomLore@ItemStackBase@@QEAAXAEBV?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@@Z",
		void, void*, vector<string>)(item, lores);
    return true;
}