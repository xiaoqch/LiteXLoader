#include <string>
#include <vector>

class Actor;
class Tag;
class FloatVec4;

Actor* Raw_SpawnItemByItemStack(ItemStack* item, const FloatVec4& pos);
Actor* Raw_SpawnItemByNBT(Tag* tag, const FloatVec4& pos);

std::string Raw_GetItemName(ItemStack* item);
std::string Raw_GetCustomName(ItemStack* item);
std::string Raw_GetItemTypeName(ItemStack* item);
int Raw_GetItemId(ItemStack* item);
int Raw_GetItemAux(ItemStack* item);
int Raw_GetCount(ItemStack* item);

bool Raw_IsNull(ItemStack* item);
bool Raw_SetLore(ItemStack* item, std::vector<std::string> lores);