#pragma once
#include <ScriptX/ScriptX.h>
#include <string>
#include <vector>
using namespace script;

//////////////////// APIs ////////////////////

Local<Value> Runcmd(const Arguments& args);
Local<Value> RuncmdEx(const Arguments& args);
Local<Value> RegisterPlayerCmd(const Arguments& args);
Local<Value> RegisterConsoleCmd(const Arguments& args);
Local<Value> SendCmdOutput(const Arguments& args);


//////////////////// LXL Event Callbacks ////////////////////

class Player;
//helper
std::vector<std::string> SplitCmdParas(const std::string& paras);

// 注册LXL内置命令
void RegisterBuiltinCmds();
// 命令回调查询
std::string LxlFindCmdReg(bool isPlayerCmd, const std::string& cmd, std::vector<std::string>& receiveParas);
// 删除指定引擎的所有命令
bool LxlRemoveCmdRegister(script::ScriptEngine* engine);

// 处理命令延迟注册
void ProcessRegCmdQueue();
// 处理调试引擎事件
bool ProcessDebugEngine(const std::string& cmd);
// 处理热管理系统
bool ProcessHotManagement(const std::string& cmd);
// 处理关服事件
void ProcessStopServer(const std::string& cmd);

// 玩家自定义命令注册回调
bool CallPlayerCmdCallback(Player* player, const std::string& cmdPrefix, const std::vector<std::string>& paras);
// 控制台自定义命令注册回调
bool CallServerCmdCallback(const std::string& cmdPrefix, const std::vector<std::string>& paras);
// 表单回调
bool CallFormCallback(Player* player, unsigned formId, const std::string& data);