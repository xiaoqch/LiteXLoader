#include "APIhelp.h"
#include "BaseAPI.h"
#include "PlayerAPI.h"
#include "../Kernel/Player.h"
#include <string>
#include <vector>
using namespace std;
using namespace script;

Local<Value> GetPlayer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        string target = args[0].asString().toString();
        auto playerList = Raw_GetAllPlayers();
        for(Player *p : playerList)
        {
            if(Raw_GetXuid(p) == target || Raw_GetPlayerName(p) == target)
                return NewPlayer(p);
        }
        return Local<Value>();    //Null
    }
    CATCH("Fail in GetPlayer!")
}

Local<Value> GetPlayerName(const Arguments& args)
{ 
    CHECK_ARGS_COUNT(args,1)

    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return String::newString(Raw_GetPlayerName(player));

        return Local<Value>(); // Null
    }
    CATCH("Fail in GetPlayerName!")
}

Local<Value> GetPlayerPos(const Arguments& args)
{ 
    CHECK_ARGS_COUNT(args,1)

    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return NewPos(Raw_GetPlayerPos(player));
            
        return Local<Value>(); // Null
    }
    CATCH("Fail in GetPlayerPos!")
}

Local<Value> GetXuid(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return String::newString(Raw_GetXuid(player));
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in GetXuid!")
}

Local<Value> GetRealName(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return String::newString(Raw_GetRealName(player));
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in GetRealName!")
}

Local<Value> GetIP(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return String::newString(Raw_GetIP(player));
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in GetIP!")
}

Local<Value> GetPlayerList(const Arguments& args)
{
    try{
        auto players = Raw_GetPlayerList();
        Local<Array> list = Array::newArray();
        for(auto p : players)
            list.add(NewPlayer(p));
        return list;
    }
    CATCH("Fail in GetPlayerList!")
}

Local<Value> TeleportPlayer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    
    try{
        FloatPos *pos = ExtractFloatPos(args[1]);
        if(!pos)
            return Local<Value>();

        Player *player = ExtractPlayer(args[0]);
        if(player)
            return Boolean::newBoolean(Raw_TeleportPlayer(player,*pos));

        return Local<Value>();    //Null
    }
    CATCH("Fail in TeleportPlayer!")
}

Local<Value> KillPlayer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return Boolean::newBoolean(Raw_KillPlayer(player));

        return Local<Value>();    //Null
    }
    CATCH("Fail in KillPlayer!")
}

Local<Value> IsOP(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return Boolean::newBoolean(Raw_GetPlayerPermLevel(player) >= 1);
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in IsOP!")
}

Local<Value> GetPlayerPermLevel(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return Number::newNumber(Raw_GetPlayerPermLevel(player));
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in GetPlayerPermLevel!")
}

Local<Value> SetPlayerPermLevel(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)
    
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
        {
            bool res = false;
            int newPerm = args[1].asNumber().toInt32();
            if(newPerm>=0 || newPerm<=4)
                res = Raw_SetPlayerPermLevel(player,newPerm);
            return Boolean::newBoolean(res);
        }
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in SetPlayerPermLevel!")
}

///////////////////////////////////////////////////// FIX HERE
Local<Value> KickPlayer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
        {
            string msg="正在从服务器断开连接";
            if(args.size() >= 2 && args[1].getKind() == ValueKind::kString)
                msg = args[1].asString().toString();
            
            return Boolean::newBoolean(Raw_KickPlayer(player,msg));
        }
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in KickPlayer!")
}

Local<Value> Tell(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)

    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
        {
            TextType type = TextType::RAW;
            if(args.size() >= 3 && args[2].getKind() == ValueKind::kNumber)
            {
                int newType = args[2].asNumber().toInt32();
                if(newType >= 0 && newType <= 9)
                    type = (TextType)newType;
            }
            return Boolean::newBoolean(Raw_Tell(player,args[1].asString().toString(),type));
        }
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in Tell!")
}

Local<Value> GetHand(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return NewItem(Raw_GetHand(player));
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in GetHand!")
}

Local<Value> RenamePlayer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
        {
            return Boolean::newBoolean(Raw_RenamePlayer(player,args[1].asString().toString()));
        }
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in RenamePlayer!")
}