#include "pch.h"
#include <string>
#include <vector>

std::string Raw_GetItemName(ItemStack* item);
std::string Raw_GetCustomName(ItemStack* item);
std::string Raw_GetItemTypeName(ItemStack* item);
int Raw_GetItemId(ItemStack* item);
int Raw_GetItemAux(ItemStack* item);
int Raw_GetCount(ItemStack* item);

bool Raw_IsNull(ItemStack* item);
bool Raw_SetLore(ItemStack* item, std::vector<std::string> lores);