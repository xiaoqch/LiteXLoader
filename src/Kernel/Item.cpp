#include "Item.h"
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

int Raw_GetItemAux(ItemStack* item)
{
    return item->getAuxValue();
}

int Raw_GetCount(ItemStack* item)
{
    ///////////////////////////////////////////////////// FIX HERE
    return WItem(*item).getCount();
}

bool Raw_IsNull(ItemStack* item)
{
    return item->isNull();
}

bool Raw_SetLore(ItemStack* item, vector<string> lores)
{
    ///////////////////////////////////////////////////// FIX HERE ?
    SymCall("?setCustomLore@ItemStackBase@@QEAAXAEBV?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@@Z",
		void, void*, vector<basic_string<char, char_traits<char>, allocator<char>>>)(item, lores);
    return true;
}