#include "Global.h"
#include <string>

std::string Raw_GetDateTimeStr();
std::string Raw_RandomGuid();
bool Raw_PathExists(const std::string &path);
bool Raw_FileReadAll(const std::string &path, std::string & result);
bool Raw_FileWriteAll(const std::string &path, const std::string &data);