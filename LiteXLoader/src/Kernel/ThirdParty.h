#pragma once

#include <Nlohmann/json.hpp>
#include <Nlohmann/fifo_map.hpp>
#include <SimpleIni/SimpleIni.h>
#include <string>
using namespace nlohmann;

template<class Key, class T, class dummy_compare, class Allocator>
using workaround_fifo_map = fifo_map<Key, T, fifo_map_compare<Key>, Allocator>;
using fifo_json = basic_json<workaround_fifo_map>;

#define JSON_NAMESPACE nlohmann
#define JSON_ROOT fifo_json
#define JSON_VALUE fifo_json

//////////////// INI Helper ////////////////
class SimpleIni : public CSimpleIniA
{
public:
	std::string filePath;
};

#define INI_ROOT SimpleIni*