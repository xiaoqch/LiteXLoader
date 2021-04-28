#include "APIhelp.h"
#include "BaseAPI.h"
#include "PlayerAPI.h"
#include <string>
#include <vector>
using namespace std;
using namespace script;

//////////////////// Hook ////////////////////

bool Hook_KickPlayer(Player* player, const string &msg)
{
    Minecraft *mc;
    auto nh = mc->getServerNetworkHandler();
    NetworkIdentifier* a = offPlayer::getNetworkIdentifier(player);
    nh->disconnectClient(*(NetworkIdentifier*)a, msg, 0);
    return true;
}

//////////////////// APIs ////////////////////

Local<Value> GetPlayer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        string target = args[0].asString().toString();
        auto playerList = liteloader::getAllPlayers();
        for(Player *p : playerList)
        {
            if(offPlayer::getXUIDString(p) == target || p->getNameTag() == target)
                return NewPlayer(p);
        }
        return Local<Value>();    //Null
    }
    CATCH("Fail in GetPlayer!")
}

Local<Value> GetXuid(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return String::newString(offPlayer::getXUIDString(player));
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
            return String::newString(offPlayer::getRealName(player));
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
            return String::newString(liteloader::getIP(*offPlayer::getNetworkIdentifier(player)));
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in GetIP!")
}

Local<Value> GetPlayerList(const Arguments& args)
{
    try{
        auto players = liteloader::getAllPlayers();
        Local<Array> list = Array::newArray();
        for(auto p : players)
            list.add(NewPlayer(p));
        return list;
    }
    CATCH("Fail in GetPlayerList!")
}

///////////////////////////////////////////////////// May have problems. 1 or 3 ?
Local<Value> IsOP(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return Boolean::newBoolean((int)WPlayer(*player).getPermLvl() == 1);
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
            return Number::newNumber((int)WPlayer(*player).getPermLvl());
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
            {
                ((ServerPlayer*)player)->setPermissions((CommandPermissionLevel)newPerm);
                res = true;
            }
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
            
            return Boolean::newBoolean(Hook_KickPlayer(player,msg));
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
            WPlayer(*player).sendText(args[1].asString().toString(),type);
            return Boolean::newBoolean(true);
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
            return NewItem((ItemStack*)&(player->getSelectedItem()));
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in GetHand!")
}

Local<Value> RunCmdAs(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return Boolean::newBoolean(liteloader::runcmdAs(player,args[1].asString().toString()));
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in RunCmdAs!")
}

Local<Value> RenamePlayer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    
    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
        {
            player->setNameTag(args[1].asString().toString());
            return Boolean::newBoolean(true);
        }
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in RenamePlayer!")
}