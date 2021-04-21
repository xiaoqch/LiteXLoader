#pragma once
#include "ScriptX.h"
#include "../LiteLoader/headers/lbpch.h"
#include "LogAPI.h"

// 至少COUNT个参数
#define CHECK_ARGS_COUNT(ARGS,COUNT) \
    if(ARGS.size()<COUNT) \
    {return Boolean::newBoolean(false);}

// 检查是否TYPE类型 
#define CHECK_ARG_TYPE(ARG,TYPE) \
    if(ARG.getKind() != TYPE) \
    {return Boolean::newBoolean(false);}

#define CATCH(LOG) \
    catch(script::Exception& e) \
    { ERROR(LOG##"\n"); PRINT(e); return Boolean::newBoolean(false);}

// 串行化
void PrintValue(std::ostream &out, script::Local<script::Value> v);