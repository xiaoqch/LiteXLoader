#include "APIhelp.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "BaseAPI.h"
#include "BlockAPI.h"
#include "EntityAPI.h"
#include "ItemAPI.h"
#include "PlayerAPI.h"
#include "../Kernel/Global.h"
#include "EngineOwnData.h"

#include "../Nlohmann/json.hpp"
#include "../Nlohmann/fifo_map.hpp"
using namespace script;
using namespace nlohmann;

template<class K, class V, class dummy_compare, class A>
using workaround_fifo_map = fifo_map<K, V, fifo_map_compare<K>, A>;
using fifo_json = basic_json<workaround_fifo_map>;

//////////////////// APIs ////////////////////

void PrintValue(std::ostream &out, Local<Value> v)
{
    switch(v.getKind())
    {
        case ValueKind::kString:
            out << v.asString().toString();
            break;
        case ValueKind::kNumber:
            out << v.asNumber().toDouble();
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
    std::shared_ptr<ScriptEngine> engine;

    #if !defined(SCRIPTX_BACKEND_WEBASSEMBLY)
        engine = std::shared_ptr<ScriptEngine>{
            new ScriptEngineImpl(),
                ScriptEngine::Deleter()
        };
    #else
        engine = std::shared_ptr<ScriptEngine>{
            ScriptEngineImpl::instance(),
                [](void*) {}
        };
    #endif

    engine->setData(make_shared<EngineOwnData>());
    return engine;
}


///////////////////// Json To Value /////////////////////

void JsonToValue_Helper(Local<Array> &res, fifo_json &j);

void JsonToValue_Helper(Local<Object> &res, const string &key, fifo_json &j)
{
    if(j.is_string())
        res.set(key, String::newString(j.get<string>()));
    else if(j.is_number_integer())
        res.set(key, Number::newNumber(j.get<int>()));
    else if(j.is_number_float())
        res.set(key, Number::newNumber(j.get<double>()));
    else if(j.is_boolean())
        res.set(key, Boolean::newBoolean(j.get<bool>()));
    else if(j.is_null())
        res.set(key, Local<Value>());
    else if(j.is_array())
    {
        Local<Array> arrToAdd = Array::newArray();
        for (fifo_json::iterator it = j.begin(); it != j.end(); ++it)
            JsonToValue_Helper(arrToAdd,*it);
        res.set(key, arrToAdd);
    }
    else if(j.is_object())
    {
        Local<Object> objToAdd = Object::newObject();
        for (fifo_json::iterator it = j.begin(); it != j.end(); ++it)
            JsonToValue_Helper(objToAdd,it.key(),it.value());
        res.set(key, objToAdd);
    }
    else
        res.set(key, Local<Value>());
}

void JsonToValue_Helper(Local<Array> &res, fifo_json &j)
{
    if(j.is_string())
        res.add(String::newString(j.get<string>()));
    else if(j.is_number_integer())
        res.add(Number::newNumber(j.get<int>()));
    else if(j.is_number_float())
        res.add(Number::newNumber(j.get<double>()));
    else if(j.is_boolean())
        res.add(Boolean::newBoolean(j.get<bool>()));
    else if(j.is_null())
        res.add(Local<Value>());
    else if(j.is_array())
    {
        Local<Array> arrToAdd = Array::newArray();
        for (fifo_json::iterator it = j.begin(); it != j.end(); ++it)
            JsonToValue_Helper(arrToAdd,*it);
        res.add(arrToAdd);
    }
    else if(j.is_object())
    {
        Local<Object> objToAdd = Object::newObject();
        for (fifo_json::iterator it = j.begin(); it != j.end(); ++it)
            JsonToValue_Helper(objToAdd,it.key(),it.value());
        res.add(objToAdd);
    }
    else
        res.add(Local<Value>());
}

Local<Value> JsonToValue(std::string jsonStr)
{
    auto j = fifo_json::parse(jsonStr);
    Local<Value> res;
    
    if(j.is_string())
        res = String::newString(j.get<string>());
    else if(j.is_number_integer())
        res = Number::newNumber(j.get<int>());
    else if(j.is_number_float())
        res = Number::newNumber(j.get<double>());
    else if(j.is_boolean())
        res = Boolean::newBoolean(j.get<bool>());
    else if(j.is_null())
        res = Local<Value>();
    else if(j.is_array())
    {
        Local<Array> resArr = Array::newArray();
        for (fifo_json::iterator it = j.begin(); it != j.end(); ++it)
            JsonToValue_Helper(resArr,*it);
        res = resArr;
    }
    else if(j.is_object())
    {
        Local<Object> resObj = Object::newObject();
        for (fifo_json::iterator it = j.begin(); it != j.end(); ++it)
            JsonToValue_Helper(resObj,it.key(),it.value());
        res = resObj;
    }
    else
        res = Local<Value>();
    
    return res;
}


///////////////////// Value To Json /////////////////////

void ValueToJson_Helper(fifo_json &res, const Local<Value> &v)
{
    switch(v.getKind())
    {
        case ValueKind::kString:
            res.push_back(v.asString().toString());
            break;
        case ValueKind::kNumber:
            res.push_back(v.asNumber().toDouble());
            break;
        case ValueKind::kBoolean:
            res.push_back(v.asBoolean().value());
            break;
        case ValueKind::kNull:
            res.push_back(nullptr);
            break;
        case ValueKind::kArray:
        {
            Local<Array> arr=v.asArray();
            if(arr.size() == 0)
                res.push_back(fifo_json::array());
            else
            {
                fifo_json arrToAdd = fifo_json::array();
                for(int i=0;i<arr.size();++i)
                {
                    fifo_json arrItem;
                    ValueToJson_Helper(arrItem,arr.get(i));
                    arrToAdd.push_back(arrItem);
                }
                res.push_back(arrToAdd);
            }
            break;
        }
        case ValueKind::kObject:
        {
            Local<Object> obj = v.asObject();
            std::vector<std::string> keys = obj.getKeyNames();
            if(keys.empty())
                res.push_back(fifo_json::object());
            else
            {
                fifo_json objToAdd = fifo_json::object();
                for(int i=0;i<keys.size();++i)
                {
                    fifo_json objItem;
                    ValueToJson_Helper(objItem,obj.get(keys[i]));
                    objToAdd.push_back({keys[i],objItem});
                }
                res.push_back(objToAdd);
            }
            break;
        }
        default:
            res.push_back(nullptr);
            break;
    }
}

std::string ValueToJson(const Local<Value> &v,int formatIndent)
{
    fifo_json res;
    ValueToJson_Helper(res,v);
    return res.dump(formatIndent);
}