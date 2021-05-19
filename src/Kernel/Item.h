#include "Global.h"
#include <string>
#include <vector>

std::string Raw_GetItemName(ItemStack* item);
std::string Raw_GetCustomName(ItemStack* item);
int Raw_GetCount(ItemStack* item);
bool Raw_SetLore(ItemStack* item, std::vector<std::string> lores);