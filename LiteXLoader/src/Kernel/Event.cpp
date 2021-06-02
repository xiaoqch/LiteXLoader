#include "Event.h"
using namespace std;

//Init Minecraft Pointer
Minecraft *mc;
THook(void, "?initAsDedicatedServer@Minecraft@@QEAAXXZ",
	void* self)
{
	original(self);
	mc = (Minecraft*)self;
}