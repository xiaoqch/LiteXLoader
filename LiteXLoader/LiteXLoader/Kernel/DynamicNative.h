#pragma once
#include <string>
#include <vector>

enum class NativeTypes : int
{
	Void, Bool, Char, UnsignedChar, Short, UnsignedShort, Int, UnsignedInt, Long, UnsignedLong, 
	LongLong, UnsignedLongLong, Float, Double,
	Pointer, CString, CWideString,
	String, WideString,  Vector, Map, UnorderedMap,
	Int8, Int16, Int32, Int64,
	HANDLE, DWORD, LPSTR, LPWSTR,

	Player, Block, Entity, ItemStack, BlockEntity, Container, Objective,
	BlockPos, Vec3
};

void* ResolveSymbol(const string& sym);
void* HookSymbol(const string& sym, void** org, void* hook);
char GetTypeSignature(NativeTypes type);