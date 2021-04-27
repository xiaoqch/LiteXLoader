#include "APIhelp.h"
#include <iostream>
#include <string>
#include <vector>

#include "BaseAPI.h"
#include "BlockAPI.h"
#include "EntityAPI.h"
#include "ItemAPI.h"
#include "PlayerAPI.h"
using namespace script;

//////////////////// APIs ////////////////////

Local<Object> NewPos(int x, int y, int z, int dim)
{
    return EngineScope::currentEngine()->newNativeClass<IntPos>(x,y,z,dim);
}
Local<Object> NewPos(double x, double y, double z, int dim)
{
    return EngineScope::currentEngine()->newNativeClass<FloatPos>(x,y,z,dim);
}
IntPos* ExtractIntPos(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<IntPos>(v))
        return EngineScope::currentEngine()->getNativeInstance<IntPos>(v);
    else
        return nullptr;
}

Local<Object> NewPos(const Vec3 &v, int dim)
{
    return NewPos(v.x,v.y,v.z,dim);
}
Local<Object> NewPos(const BlockPos &b, int dim)
{
    return NewPos(b.x,b.y,b.z,dim);
}
FloatPos* ExtractFloatPos(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<FloatPos>(v))
        return EngineScope::currentEngine()->getNativeInstance<FloatPos>(v);
    else
        return nullptr;
}

Local<Object> NewPlayer(Player *p)
{
  auto newp = new PlayerPointer(p);
  return newp->getScriptObject();
}
Local<Object> NewPlayer(WPlayer p)
{
  auto newp = new PlayerPointer(p.v);
  return newp->getScriptObject();
}
Player* ExtractPlayer(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<PlayerPointer>(v))
        return EngineScope::currentEngine()->getNativeInstance<PlayerPointer>(v)->get();
    else
        return nullptr;
}

Local<Object> NewEntity(Actor *p)
{
  auto newp = new EntityPointer(p);
  return newp->getScriptObject();
}
Local<Object> NewEntity(WActor p)
{
  auto newp = new EntityPointer(p.v);
  return newp->getScriptObject();
}
Actor* ExtractEntity(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<EntityPointer>(v))
        return EngineScope::currentEngine()->getNativeInstance<EntityPointer>(v)->get();
    else
        return nullptr;
}

Local<Object> NewBlock(Block *p)
{
  auto newp = new BlockPointer(p);
  return newp->getScriptObject();
}
Local<Object> NewBlock(WBlock p)
{
  auto newp = new BlockPointer(p.v);
  return newp->getScriptObject();
}
Block* ExtractBlock(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<BlockPointer>(v))
        return EngineScope::currentEngine()->getNativeInstance<BlockPointer>(v)->get();
    else
        return nullptr;
}

Local<Object> NewItem(ItemStack *p)
{
  auto newp = new ItemPointer(p);
  return newp->getScriptObject();
}
ItemStack* ExtractItem(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<ItemPointer>(v))
        return EngineScope::currentEngine()->getNativeInstance<ItemPointer>(v)->get();
    else
        return nullptr;
}

void PrintValue(std::ostream &out, Local<Value> v)
{
    switch(v.getKind())
    {
        case ValueKind::kString:
            out << v.asString().toString();
            break;
        case ValueKind::kNumber:
            out << v.asNumber().toInt32();
            break;
        case ValueKind::kBoolean:
            out << v.asBoolean().value();
            break;
        case ValueKind::kNull:
            out << "<Null>";
            break;
        case ValueKind::kArray:
        {
            Local<Array> arr=v.asArray();
            if(arr.size() == 0)
                out << "[]";
            else
            {
                out << '[';
                PrintValue(out,arr.get(0));
                for(int i=1;i<arr.size();++i)
                {
                    out << ',';
                    PrintValue(out,arr.get(i));
                }
                out << ']';
            }
            break;
        }
        case ValueKind::kObject:
        {
            Local<Object> obj = v.asObject();
            std::vector<std::string> keys = obj.getKeyNames();
            if(keys.empty())
                out << "{}";
            else
            {
                out << '{';
                out << keys[0]+":";
                PrintValue(out,obj.get(keys[0]));
                for(int i=1;i<keys.size();++i)
                {
                    out << ","+keys[i]+":";
                    PrintValue(out,obj.get(keys[i]));
                }
                out << '}';
            }
            break;
        }
        case ValueKind::kByteBuffer:
        {
            Local<ByteBuffer> bytes = v.asByteBuffer();
            out.write((const char*)bytes.getRawBytes(),bytes.byteLength());
            break;
        }
        case ValueKind::kFunction:
            out << "<Function>";
            break;
        default:
            out << "<Unknown>";
            break;
    }
}

std::shared_ptr<ScriptEngine> NewEngine()
{
    #if !defined(SCRIPTX_BACKEND_WEBASSEMBLY)
        return std::shared_ptr<ScriptEngine>{
            new ScriptEngineImpl(),
                ScriptEngine::Deleter()
        };
    #else
        return std::shared_ptr<ScriptEngine>{
            ScriptEngineImpl::instance(),
                [](void*) {}
        };
    #endif
}