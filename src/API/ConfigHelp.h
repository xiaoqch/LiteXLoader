#include "../Configs.h"
#include <string>

namespace conf
{
	std::string getString(const std::string &sec, const std::string &key,const std::string &default = "");
	int getInt(const std::string &sec, const std::string &key, int default = 0);
    bool getBool(const std::string &sec, const std::string &key, bool default = false);
}