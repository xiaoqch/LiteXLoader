#pragma once
#include "../Kernel/Global.h"
#include "../Kernel/ThirdParty.h"
#include "ScriptX.h"
#include "../Configs.h"
#include "BaseAPI.h"
#include <string>
using namespace script;


// 输出
#define PREFIX "[LiteXLoader." ## LXL_SCRIPT_LANG_TYPE ## "]"
#define DEBUG(t)  std::cout << PREFIX ## "[Debug] " << (t) << std::endl
#define INFO(t)  std::cout << PREFIX ## "[Info] " << (t) << std::endl
#define WARN(t)  std::cout << PREFIX ## "[Warning] " << (t) << std::endl
#define ERROR(t)  std::cerr << PREFIX ## "[Error] " << (t) << std::endl
#define FATAL(t)  std::cerr << PREFIX ## "[FATAL] " << (t) << std::endl
#define PRINT(t)  std::cout << t << std::endl
#define ERRPRINT(t)  std::cerr << t << std::endl

//方便提取类型
#define toStr() asString().toString()
#define toInt() asNumber().toInt32()

// 至少COUNT个参数
#define CHECK_ARGS_COUNT(ARGS,COUNT) \
    if(ARGS.size()<COUNT) \
    {return Local<Value>();}

// 检查是否TYPE类型 
#define CHECK_ARG_TYPE(ARG,TYPE) \
    if(ARG.getKind() != TYPE) \
    {return Local<Value>();}

// 截获引擎异常
#define CATCH(LOG) \
    catch(Exception& e) \
    { ERROR(LOG##"\n"); ERRPRINT(e); return Local<Value>();}

// 创建新引擎
std::shared_ptr<ScriptEngine> NewEngine();


// 序列化
void PrintValue(std::ostream &out, Local<Value> v);

// Json 序列化 反序列化
Local<Value> JsonToValue(std::string jsonStr);
Local<Value> JsonToValue(JSON_VALUE j);
std::string ValueToJson(Local<Value> v,int formatIndent = -1);