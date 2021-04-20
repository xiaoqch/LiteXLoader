#include "APIhelp.h"
#include <iostream>
#include <string>
#include <vector>
using namespace script;

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