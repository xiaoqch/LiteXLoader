#pragma once
#include "ScriptX.h"
#include "../Kernel/Global.h"
#include "../Configs.h"
#include "BaseAPI.h"
using namespace script;

// 输出
#define PREFIX "[LiteXLoader." ## LXL_SCRIPT_LANG_TYPE ## "]"
#define DEBUG(t)  std::cout << PREFIX ## "[Debug] " << t << std::endl
#define INFO(t)  std::cout << PREFIX ## "[Info] " << t << std::endl
#define WARN(t)  std::cout << PREFIX ## "[Warning] " << t << std::endl
#define ERROR(t)  std::cerr << PREFIX ## "[Error] " << t << std::endl
#define FATAL(t)  std::cerr << PREFIX ## "[FATAL] " << t << std::endl
#define PRINT(t)  std::cout << t << std::endl
#define ERRPRINT(t)  std::cerr << t << std::endl

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

// 串行化
void PrintValue(std::ostream &out, Local<Value> v);

//创建新引擎
std::shared_ptr<ScriptEngine> NewEngine();

// 通用坐标类
Local<Object> NewPos(int x, int y, int z, int dim = -1);
Local<Object> NewPos(const BlockPos &v, int dim = -1);
Local<Object> NewPos(const IntVec4 &v);
IntPos* ExtractIntPos(Local<Value> v);

Local<Object> NewPos(double x, double y, double z, int dim = -1);
Local<Object> NewPos(const Vec3 &v, int dim = -1);
Local<Object> NewPos(const FloatVec4 &v);
FloatPos* ExtractFloatPos(Local<Value> v);

//对象指针类
Local<Object> NewPlayer(Player *p);
Local<Object> NewPlayer(WPlayer p);
Player* ExtractPlayer(Local<Value> v);

Local<Object> NewEntity(Actor *p);
Local<Object> NewEntity(WActor p);
Actor* ExtractEntity(Local<Value> v);

Local<Object> NewBlock(Block *p);
Local<Object> NewBlock(WBlock p);
Block* ExtractBlock(Local<Value> v);

Local<Object> NewItem(ItemStack *p);
ItemStack* ExtractItem(Local<Value> v);