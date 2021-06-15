#include "APIhelp.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>

#include "BaseAPI.h"
#include "BlockAPI.h"
#include "EntityAPI.h"
#include "ItemAPI.h"
#include "PlayerAPI.h"
#include <Kernel/Global.h>
#include "EngineOwnData.h"
using namespace script;

//////////////////// APIs ////////////////////

void PrintValue(std::ostream &out, Local<Value> v)
{
    switch(v.getKind())
    {
        case ValueKind::kString:
            out << v.asString().toString();
            break;
        case ValueKind::kNumber:
            if(CheckIsFloat(v))
                out << v.asNumber().toDouble();
            else
                out << v.asNumber().toInt64();
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

bool CheckIsFloat(const Local<Value> &num)
{
    try
    {
        return fabs(num.asNumber().toDouble() - num.asNumber().toInt64()) >= 0.000001;
    }
    catch (...)
    {
        return false;
    }
}


///////////////////// Json To Value /////////////////////

void JsonToValue_Helper(Local<Array> &res, JSON_VALUE &j);

void JsonToValue_Helper(Local<Object> &res, const string &key, JSON_VALUE &j)
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
        for (JSON_VALUE::iterator it = j.begin(); it != j.end(); ++it)
            JsonToValue_Helper(arrToAdd,*it);
        res.set(key, arrToAdd);
    }
    else if(j.is_object())
    {
        Local<Object> objToAdd = Object::newObject();
        for (JSON_VALUE::iterator it = j.begin(); it != j.end(); ++it)
            JsonToValue_Helper(objToAdd,it.key(),it.value());
        res.set(key, objToAdd);
    }
    else
        res.set(key, Local<Value>());
}

void JsonToValue_Helper(Local<Array> &res, JSON_VALUE &j)
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
        for (JSON_VALUE::iterator it = j.begin(); it != j.end(); ++it)
            JsonToValue_Helper(arrToAdd,*it);
        res.add(arrToAdd);
    }
    else if(j.is_object())
    {
        Local<Object> objToAdd = Object::newObject();
        for (JSON_VALUE::iterator it = j.begin(); it != j.end(); ++it)
            JsonToValue_Helper(objToAdd,it.key(),it.value());
        res.add(objToAdd);
    }
    else
        res.add(Local<Value>());
}

Local<Value> JsonToValue(JSON_VALUE j)
{
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
        for (JSON_VALUE::iterator it = j.begin(); it != j.end(); ++it)
            JsonToValue_Helper(resArr,*it);
        res = resArr;
    }
    else if(j.is_object())
    {
        Local<Object> resObj = Object::newObject();
        for (JSON_VALUE::iterator it = j.begin(); it != j.end(); ++it)
            JsonToValue_Helper(resObj,it.key(),it.value());
        res = resObj;
    }
    else
        res = Local<Value>();
    
    return res;
}

Local<Value> JsonToValue(std::string jsonStr)
{
    auto j = JSON_VALUE::parse(jsonStr);
    return JsonToValue(j);
}


///////////////////// Value To Json /////////////////////

void ValueToJson_Obj_Helper(JSON_VALUE& res, const Local<Object>& v);

void ValueToJson_Arr_Helper(JSON_VALUE &res, const Local<Array> &v)
{
    for (int i = 0; i < v.size(); ++i)
    {
        switch (v.get(i).getKind())
        {
        case ValueKind::kString:
            res.push_back(v.get(i).asString().toString());
            break;
        case ValueKind::kNumber:
            if (CheckIsFloat(v))
                res.push_back(v.get(i).asNumber().toDouble());
            else
                res.push_back(v.get(i).asNumber().toInt64());
            break;
        case ValueKind::kBoolean:
            res.push_back(v.get(i).asBoolean().value());
            break;
        case ValueKind::kNull:
            res.push_back(nullptr);
            break;
        case ValueKind::kArray:
        {
            Local<Array> arrToAdd = v.get(i).asArray();
            if (arrToAdd.size() == 0)
                res.push_back(JSON_VALUE::array());
            else
            {
                JSON_VALUE arrJson = JSON_VALUE::array();
                ValueToJson_Arr_Helper(arrJson, arrToAdd);
                res.push_back(arrJson);
            }
            break;
        }
        case ValueKind::kObject:
        {
            Local<Object> objToAdd = v.get(i).asObject();
            if (objToAdd.getKeyNames().empty())
                res.push_back(JSON_VALUE::object());
            else
            {
                JSON_VALUE objJson = JSON_VALUE::object();
                ValueToJson_Obj_Helper(objJson, objToAdd);
                res.push_back(objJson);
            }
            break;
        }
        default:
            res.push_back(nullptr);
            break;
        }
    }
}

void ValueToJson_Obj_Helper(JSON_VALUE& res, const Local<Object>& v)
{
    auto keys = v.getKeyNames();
    for (auto &key : keys)
    {
        switch (v.get(key).getKind())
        {
        case ValueKind::kString:
            res.push_back({ key,v.get(key).asString().toString() });
            break;
        case ValueKind::kNumber:
            if (CheckIsFloat(v))
                res.push_back({ key,v.get(key).asNumber().toDouble() });
            else
                res.push_back({ key,v.get(key).asNumber().toInt64() });
            break;
        case ValueKind::kBoolean:
            res.push_back({ key,v.get(key).asBoolean().value() });
            break;
        case ValueKind::kNull:
            res.push_back(nullptr);
            break;
        case ValueKind::kArray:
        {
            Local<Array> arrToAdd = v.get(key).asArray();
            if (arrToAdd.size() == 0)
                res.push_back({ key,JSON_VALUE::array() });
            else
            {
                JSON_VALUE arrJson = JSON_VALUE::array();
                ValueToJson_Arr_Helper(arrJson, arrToAdd);
                res.push_back({ key,arrJson });
            }
            break;
        }
        case ValueKind::kObject:
        {
            Local<Object> objToAdd = v.get(key).asObject();
            if (objToAdd.getKeyNames().empty())
                res.push_back({ key,JSON_VALUE::object() });
            else
            {
                JSON_VALUE objJson = JSON_VALUE::object();
                ValueToJson_Obj_Helper(objJson, objToAdd);
                res.push_back({ key,objJson });
            }
            break;
        }
        default:
            res.push_back({ key,nullptr });
            break;
        }
    }
}

std::string ValueToJson(Local<Value> v,int formatIndent)
{
    string res;
    switch (v.getKind())
    {
    case ValueKind::kString:
        res = v.asString().toString();
        break;
    case ValueKind::kNumber:
        if (CheckIsFloat(v))
            res = std::to_string(v.asNumber().toDouble());
        else
            res = std::to_string(v.asNumber().toInt64());
        break;
    case ValueKind::kBoolean:
        res = std::to_string(v.asBoolean().value());
        break;
    case ValueKind::kNull:
        res = "";
        break;
    case ValueKind::kArray:
    {
        JSON_VALUE jsonRes = JSON_VALUE::array();
        ValueToJson_Arr_Helper(jsonRes, v.asArray());
        res = jsonRes.dump(formatIndent);
        break;
    }
    case ValueKind::kObject:
    {
        JSON_VALUE jsonRes = JSON_VALUE::object();
        ValueToJson_Obj_Helper(jsonRes, v.asObject());
        res = jsonRes.dump(formatIndent);
        break;
    }
    default:
        res = "";
        break;
    }
    return res;
}