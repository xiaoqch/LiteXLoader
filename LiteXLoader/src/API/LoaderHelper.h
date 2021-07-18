#pragma once
#include <string>

std::string ReadFileFrom(const std::string& filePath);
std::shared_ptr<ScriptEngine> NewEngine();

bool LxlLoadPlugin(const std::string& filePath);