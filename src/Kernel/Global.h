#pragma once

#include "../LiteLoader/headers/lbpch.h"
#include "../LiteLoader/headers/mc/OffsetHelper.h"
#include "../LiteLoader/headers/mc/block.h"
#include "../LiteLoader/headers/mc/item.h"
#include "../LiteLoader/headers/mc/mass.h"
#include "../LiteLoader/headers/api/commands.h"
#include "../LiteLoader/headers/api/types/types.h"
#include "../LiteLoader/headers/stl/KVDB.h"
#include "../LiteLoader/headers/api/gui/gui.h"
#include "../LiteLoader/headers/httplib.h"

class IntVec4
{
public:
	int x,y,z;
    int dim;
};

class FloatVec4
{
public:
	float x,y,z;
    int dim;
};