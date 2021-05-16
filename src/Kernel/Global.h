#pragma once

#include "../LiteLoader/headers/lbpch.h"
#include "../LiteLoader/headers/mc/OffsetHelper.h"
#include "../LiteLoader/headers/mc/block.h"
#include "../LiteLoader/headers/mc/item.h"
#include "../LiteLoader/headers/mc/mass.h"
#include "../LiteLoader/headers/api/commands.h"

class IntVec4
{
public:
	long long x,y,z;
    int dim;
};

class FloatVec4
{
public:
	float x,y,z;
    int dim;
};