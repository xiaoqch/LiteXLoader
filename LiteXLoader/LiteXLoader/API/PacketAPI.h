#pragma once
#include <ScriptX/ScriptX.h>
#include <stl/Bstream.h>
using namespace script;

class PacketClass : public ScriptClass
{
public:
    WBStream wb;
    int Packetid;

public:
    explicit PacketClass(int id);

    static WBStream extractPacket(Local<Value> v);
    static Local<Object> newPacket(int id);
    static int _getPacketid(Local<Value> v);

    static Local<Value> makePacket(const Arguments& args);

    Local<Value> getPacketId(const Arguments& args);

    Local<Value> writeUShort(const Arguments& args);
    Local<Value> writeInt(const Arguments& args);
    Local<Value> writeUInt(const Arguments& args);
    Local<Value> writeULong(const Arguments& args);
    Local<Value> writeString(const Arguments& args);
    Local<Value> writeFloat(const Arguments& args);
    Local<Value> writeDouble(const Arguments& args);
    Local<Value> writeBool(const Arguments& args);
};
extern ClassDefine<PacketClass> PacketClassBuilder;

