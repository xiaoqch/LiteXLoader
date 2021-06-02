#pragma once

#include <LiteLoader/lbpch.h>
#include <LiteLoader/mc/OffsetHelper.h>
#include <LiteLoader/mc/block.h>
#include <LiteLoader/mc/item.h>
#include <LiteLoader/mc/mass.h>
#include <LiteLoader/api/commands.h>
#include <LiteLoader/api/types/types.h>
#include <LiteLoader/stl/KVDB.h>
#include <LiteLoader/api/gui/gui.h>
#include <LiteLoader/httplib.h>
#include <LiteLoader/api/myPacket.h>

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