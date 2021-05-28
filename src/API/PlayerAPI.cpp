#include "APIhelp.h"
#include "BaseAPI.h"
#include "PlayerAPI.h"
#include "ItemAPI.h"
#include "EngineOwnData.h"
#include "../Kernel/Player.h"
#include "../Kernel/Entity.h"
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<PlayerClass> PlayerClassBuilder =
    defineClass<PlayerClass>("Player")
        .constructor(nullptr)
        .instanceProperty("name", &PlayerClass::getName)
        .instanceProperty("pos", &PlayerClass::getPos)
        .instanceProperty("realName", &PlayerClass::getRealName)
        .instanceProperty("xuid", &PlayerClass::getXuid)
        .instanceProperty("ip", &PlayerClass::getIP)
        .instanceProperty("maxHealth", &PlayerClass::getMaxHealth)
        .instanceProperty("health", &PlayerClass::getHealth)

        .instanceFunction("isOP", &PlayerClass::isOP)
        .instanceFunction("getPlayerPermLevel", &PlayerClass::getPlayerPermLevel)
        .instanceFunction("setPlayerPermLevel", &PlayerClass::setPlayerPermLevel)

        .instanceFunction("runcmdAs", &PlayerClass::runcmdAs)
        .instanceFunction("teleport", &PlayerClass::teleport)
        .instanceFunction("kill", &PlayerClass::kill)
        .instanceFunction("kick", &PlayerClass::kick)
        .instanceFunction("tell", &PlayerClass::tell)
        .instanceFunction("getHand", &PlayerClass::getHand)
        .instanceFunction("getPack", &PlayerClass::getPack)
        .instanceFunction("rename", &PlayerClass::rename)

        .instanceFunction("setExtraData", &PlayerClass::setExtraData)
        .instanceFunction("getExtraData", &PlayerClass::getExtraData)
        .instanceFunction("delExtraData", &PlayerClass::delExtraData)
        .build();


//////////////////// Classes ////////////////////

//生成函数
Local<Object> PlayerClass::newPlayer(Player *p)
{
    auto newp = new PlayerClass(p);
    return newp->getScriptObject();
}
Local<Object> PlayerClass::newPlayer(WPlayer p)
{
    return PlayerClass::newPlayer(p.v);
}
Player* PlayerClass::extractPlayer(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<PlayerClass>(v))
        return EngineScope::currentEngine()->getNativeInstance<PlayerClass>(v)->get();
    else
        return nullptr;
}

//公用API
Local<Value> GetPlayer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        string target = args[0].asString().toString();
        auto playerList = Raw_GetOnlinePlayers();
        for(Player *p : playerList)
        {
            if(Raw_GetXuid(p) == target || Raw_GetPlayerName(p) == target)
                return PlayerClass::newPlayer(p);
        }
        return Local<Value>();    //Null
    }
    CATCH("Fail in GetPlayer!")
}

Local<Value> GetOnlinePlayers(const Arguments& args)
{
    try{
        auto players = Raw_GetOnlinePlayers();
        Local<Array> list = Array::newArray();
        for(auto p : players)
            list.add(PlayerClass::newPlayer(p));
        return list;
    }
    CATCH("Fail in GetOnlinePlayers!")
}

//成员函数
Local<Value> PlayerClass::getName()
{ 
    try{
        return String::newString(Raw_GetPlayerName(player));
    }
    CATCH("Fail in GetPlayerName!")
}

Local<Value> PlayerClass::getPos()
{ 
    try{
        return FloatPos::newPos(Raw_GetPlayerPos(player));
    }
    CATCH("Fail in GetPlayerPos!")
}

Local<Value> PlayerClass::getXuid()
{
    try{
        return String::newString(Raw_GetXuid(player));
    }
    CATCH("Fail in GetXuid!")
}

Local<Value> PlayerClass::getRealName()
{
    try{
        return String::newString(Raw_GetRealName(player));
    }
    CATCH("Fail in GetRealName!")
}

Local<Value> PlayerClass::getIP()
{
    try{
        return String::newString(Raw_GetIP(player));
    }
    CATCH("Fail in GetIP!")
}

Local<Value> PlayerClass::getMaxHealth()
{
    try{
        return Number::newNumber(Raw_GetMaxHealth((Actor*)player));
    }
    CATCH("Fail in GetMaxHealth!")
}

Local<Value> PlayerClass::getHealth()
{
    try{
        return Number::newNumber(Raw_GetHealth((Actor*)player));
    }
    CATCH("Fail in GetHealth!")
}

Local<Value> PlayerClass::teleport(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        FloatPos *pos = FloatPos::extractPos(args[0]);
        if(!pos)
            return Local<Value>();
        
        return Boolean::newBoolean(Raw_TeleportPlayer(player,*pos));
    }
    CATCH("Fail in TeleportPlayer!")
}

Local<Value> PlayerClass::kill(const Arguments& args)
{
    try{
        return Boolean::newBoolean(Raw_KillPlayer(player));
    }
    CATCH("Fail in KillPlayer!")
}

Local<Value> PlayerClass::isOP(const Arguments& args)
{
    try{
        return Boolean::newBoolean(Raw_GetPlayerPermLevel(player) >= 1);
    }
    CATCH("Fail in IsOP!")
}

Local<Value> PlayerClass::getPlayerPermLevel(const Arguments& args)
{
    try{
        return Number::newNumber(Raw_GetPlayerPermLevel(player));
    }
    CATCH("Fail in GetPlayerPermLevel!")
}

Local<Value> PlayerClass::setPlayerPermLevel(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kNumber)
    
    try{
        bool res = false;
        int newPerm = args[0].asNumber().toInt32();
        if(newPerm>=0 || newPerm<=4)
            res = Raw_SetPlayerPermLevel(player,newPerm);
        return Boolean::newBoolean(res);
    }
    CATCH("Fail in SetPlayerPermLevel!")
}

Local<Value> PlayerClass::runcmdAs(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    
    try{
        return Boolean::newBoolean(Raw_RuncmdAs(player,args[0].asString().toString()));
    }
    CATCH("Fail in RunCmdAs!")
}

Local<Value> PlayerClass::kick(const Arguments& args)
{
    try{
        string msg="正在从服务器断开连接";
        if(args.size() >= 1 && args[0].getKind() == ValueKind::kString)
            msg = args[0].asString().toString();
        
        return Boolean::newBoolean(Raw_KickPlayer(player,msg));
    }
    CATCH("Fail in KickPlayer!")
}

Local<Value> PlayerClass::tell(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        TextType type = TextType::RAW;
        if(args.size() >= 2 && args[1].getKind() == ValueKind::kNumber)
        {
            int newType = args[1].asNumber().toInt32();
            if(newType >= 0 && newType <= 9)
                type = (TextType)newType;
        }
        return Boolean::newBoolean(Raw_Tell(player,args[1].asString().toString(),type));
    }
    CATCH("Fail in Tell!")
}

Local<Value> PlayerClass::getHand(const Arguments& args)
{
    try{
        return ItemClass::newItem(Raw_GetHand(player));
    }
    CATCH("Fail in GetHand!")
}

Local<Value> PlayerClass::getPack(const Arguments& args)
{
    try{
        auto res = Raw_GetPack(player);
        Local<Array> pack = Array::newArray();
        for(ItemStack* item : res)
        {
            pack.add(ItemClass::newItem(item));
        }
        return pack;
    }
    CATCH("Fail in GetPack!")
}

Local<Value> PlayerClass::rename(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    
    try{
        return Boolean::newBoolean(Raw_RenamePlayer(player,args[0].asString().toString()));
    }
    CATCH("Fail in RenamePlayer!")
}

Local<Value> PlayerClass::setExtraData(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    string key = args[0].asString().toString();
    if(key.empty())
        return Boolean::newBoolean(false);
    
    ENGINE_OWN_DATA()->playerDataDB[Raw_GetPlayerName(player) + "-" + key] = args[1];
    return Boolean::newBoolean(true);
}

Local<Value> PlayerClass::getExtraData(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    string key = args[0].asString().toString();
    if(key.empty())
        return Boolean::newBoolean(false);

    try
    {
        return ENGINE_OWN_DATA()->playerDataDB.at(Raw_GetPlayerName(player) + "-" + key).get();
    }
    catch(...)
    {
        ERROR("Fail in getExtraData");
        return Local<Value>();  //Null
    }
    
}

Local<Value> PlayerClass::delExtraData(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    string key = args[0].asString().toString();
    if(key.empty())
        return Boolean::newBoolean(false);
    
    ENGINE_OWN_DATA()->playerDataDB.erase(Raw_GetPlayerName(player) + "-" + key);
    return Boolean::newBoolean(true);
}
