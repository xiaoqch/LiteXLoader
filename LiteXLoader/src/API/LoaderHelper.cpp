#include "LoaderHelper.h"
#include <Loader.h>

bool LxlLoadPlugin(const std::string& filePath)
{
	return LoadPlugin(filePath);
}