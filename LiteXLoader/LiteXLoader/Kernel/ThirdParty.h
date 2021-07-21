#pragma once

//////////////// Json ////////////////
#include <Nlohmann/json.hpp>
#include <Nlohmann/fifo_map.hpp>
#include <string>
using namespace nlohmann;

template<class Key, class T, class dummy_compare, class Allocator>
using workaround_fifo_map = fifo_map<Key, T, fifo_map_compare<Key>, Allocator>;
using fifo_json = basic_json<workaround_fifo_map>;

#define JSON_NAMESPACE nlohmann
#define JSON_ROOT fifo_json
#define JSON_VALUE fifo_json

//////////////// INI Helper ////////////////
#include <SimpleIni/SimpleIni.h>

class SimpleIni : public CSimpleIniA
{
public:
	std::string filePath;
};

#define INI_ROOT SimpleIni*

//////////////// DynCall ////////////////
#include <dyncall/include/dyncall.h>
#include <dyncall/include/dyncall_alloc_wx.h>
#include <dyncall/include/dyncall_args.h>
#include <dyncall/include/dyncall_callback.h>
#include <dyncall/include/dyncall_config.h>
#include <dyncall/include/dyncall_macros.h>
#include <dyncall/include/dyncall_signature.h>
#include <dyncall/include/dyncall_types.h>
#include <dyncall/include/dyncall_value.h>

//////////////// SEH Exception ////////////////
#include <seh_exception/seh_exception.hpp>