#include "Global.h"
#include "System.h"
#include <Engine/LocalShareData.h>
#include "DynamicNative.h"
using namespace std;

void* HookSymbol(const string &sym, void** org, void* hook)
{
    void* found = dlsym_real(sym.c_str());
    if (!found)
    {
        ERROR("Failed to hook " + sym);
        return nullptr;
    }
    HookFunction(found, org, hook);
    return org;
}

void* ResolveSymbol(const string& sym)
{
    return dlsym_real(sym.c_str());
}

char GetTypeSignature(NativeTypes type)
{
	switch (type)
	{
	case NativeTypes::Bool:
		return 'B';
		break;
	case NativeTypes::Char:
	case NativeTypes::Int8:
		return 'c';
		break;
	case NativeTypes::UnsignedChar:
		return 'C';
		break;
	case NativeTypes::Short:
	case NativeTypes::Int16:
		return 's';
		break;
	case NativeTypes::UnsignedShort:
		return 'S';
		break;
	case NativeTypes::Int:
	case NativeTypes::Int32:
		return 'i';
		break;
	case NativeTypes::UnsignedInt:
		return 'I';
		break;
	case NativeTypes::Long:
		return 'j';
		break;
	case NativeTypes::UnsignedLong:
		return 'J';
		break;
	case NativeTypes::LongLong:
	case NativeTypes::Int64:
		return 'l';
		break;
	case NativeTypes::UnsignedLongLong:
		return 'L';
		break;
	case NativeTypes::Float:
		return 'f';
		break;
	case NativeTypes::Double:
		return 'd';
		break;
	case NativeTypes::Pointer:
	case NativeTypes::String:
		return 'p';
		break;
		// Custom Classes
	case NativeTypes::Player:
	case NativeTypes::Block:
	case NativeTypes::Entity:
	case NativeTypes::ItemStack:
	case NativeTypes::BlockEntity:
	case NativeTypes::Container:
		return 'p';
		break;
	default:
		return 'v';
		break;
	}
}