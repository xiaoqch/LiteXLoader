#pragma once
#include <string>
#include <vector>

class Container;
class ItemStack;

bool Raw_AddItem(Container* container, ItemStack* item);
bool Raw_AddItemToFirstEmptySlot(Container* container, ItemStack* item);
bool Raw_HasRoomFor(Container* container, ItemStack* item);
bool Raw_RemoveItem(Container* container, int slot, int number);
ItemStack* Raw_GetSlot(Container* container, int slot);
std::vector<ItemStack*> Raw_GetAllSlots(Container* container);
bool Raw_RemoveAllItems(Container* container);
bool Raw_IsEmpty(Container* container);
int Raw_GetContainerSize(Container* container);