#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;

#include <Kernel/Global.h>
#include <Kernel/ThirdParty.h>
#include <Kernel/i18n.h>
#include <Engine/EngineOwnData.h>
#include <string>
#include <vector>

// 输出
extern int lxlLogLevel;
#define PREFIX "[LiteXLoader." LXL_SCRIPT_LANG_TYPE "]" 
#define DEBUG(t) { if(lxlLogLevel >= 5) std::cout << PREFIX "[Debug] " << (t) << std::endl; }
#define INFO(t)  { if(lxlLogLevel >= 4) std::cout << PREFIX "[Info] " << (t) << std::endl; }
#define WARN(t)  { if(lxlLogLevel >= 3) std::cout << PREFIX "[Warning] " << (t) << std::endl; }
#define ERROR(t)  { if(lxlLogLevel >= 2) std::cerr << PREFIX "[Error] " << (t) << std::endl; }
#define FATAL(t)  { if(lxlLogLevel >= 1) std::cerr << PREFIX "[FATAL] " << (t) << std::endl; }
#define PRINT(t)  { std::cout << t << std::endl; }
#define ERRPRINT(t)  { if(lxlLogLevel >= 2) std::cerr << t << std::endl; }

// 方便提取类型
#define toStr() asString().toString()
#define toInt() asNumber().toInt32()

// 至少COUNT个参数
#define CHECK_ARGS_COUNT(ARGS,COUNT) \
    if(ARGS.size()<COUNT) \
    {ERROR("Too Few arguments!"); return Local<Value>();}

// 检查是否TYPE类型 
#define CHECK_ARG_TYPE(ARG,TYPE) \
    if(ARG.getKind() != TYPE) \
    {ERROR("Wrong type of argument!"); return Local<Value>();}

// 判断是否为浮点数
bool CheckIsFloat(const Local<Value> &num);

// 截获引擎异常
#define CATCH(LOG) \
    catch(const seh_exception &e) \
    { ERROR("SEH Exception Caught!"); ERRPRINT(e.what()); ERRPRINT("[Error] In Plugin: " + ENGINE_OWN_DATA()->pluginName); return Local<Value>(); } \
    catch(const Exception& e) \
    { ERROR(LOG##"\n"); ERRPRINT(e); ERRPRINT("[Error] In Plugin: " + ENGINE_OWN_DATA()->pluginName); return Local<Value>();}


// 序列化
void PrintValue(std::ostream &out, Local<Value> v);
std::string ValueToString(Local<Value> v);

// Json 序列化 反序列化
Local<Value> JsonToValue(std::string jsonStr);
Local<Value> JsonToValue(JSON_VALUE j);
std::string ValueToJson(Local<Value> v,int formatIndent = -1);