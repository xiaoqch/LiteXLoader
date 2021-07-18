#pragma once
#include <string>
#include <vector>

std::string ReadFileFrom(const std::string& filePath);
ScriptEngine* NewEngine();

bool LxlLoadPlugin(const std::string& filePath);
std::string LxlUnloadPlugin(const std::string& name);
bool LxlReloadPlugin(const std::string& name);
bool LxlReloadAllPlugin(const std::string& name);
std::vector<std::string> LxlListAllPlugins();