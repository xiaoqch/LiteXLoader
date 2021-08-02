#include "APIHelp.h"
#include "BaseAPI.h"
#include "DeviceAPI.h"
#include "PlayerAPI.h"
#include "ItemAPI.h"
#include "GuiAPI.h"
#include "NbtAPI.h"
#include <Engine/EngineOwnData.h>
#include <Engine/GlobalShareData.h>
#include <Kernel/Player.h>
#include <Kernel/Entity.h>
#include <Kernel/Gui.h>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<PlayerClass> PlayerClassBuilder =
    defineClass<PlayerClass>("LXL_Player")
        .constructor(nullptr)
        .instanceFunction("getRawPtr", &PlayerClass::getRawPtr)

        .instanceProperty("name", &PlayerClass::getName)
        .instanceProperty("pos", &PlayerClass::getPos)
        .instanceProperty("realName", &PlayerClass::getRealName)
        .instanceProperty("xuid", &PlayerClass::getXuid)
        .instanceProperty("uuid", &PlayerClass::getUuid)
        .instanceProperty("ip", &PlayerClass::getIP)
        .instanceProperty("permLevel", &PlayerClass::getPermLevel)
        .instanceProperty("gameMode", &PlayerClass::getGameMode)
        .instanceProperty("maxHealth", &PlayerClass::getMaxHealth)
        .instanceProperty("health", &PlayerClass::getHealth)
        .instanceProperty("inAir", &PlayerClass::getInAir)
        .instanceProperty("sneaking", &PlayerClass::getSneaking)
        .instanceProperty("speed",&PlayerClass::getSpeed)
        .instanceProperty("direction", &PlayerClass::getDirection)

        .instanceFunction("isOP", &PlayerClass::isOP)
        .instanceFunction("setPermLevel", &PlayerClass::setPermLevel)
        .instanceFunction("setGameMode", &PlayerClass::setGameMode)

        .instanceFunction("runcmd", &PlayerClass::runcmd)
        .instanceFunction("teleport", &PlayerClass::teleport)
        .instanceFunction("kill", &PlayerClass::kill)
        .instanceFunction("kick", &PlayerClass::kick)
        .instanceFunction("disconnect", &PlayerClass::kick)
        .instanceFunction("tell", &PlayerClass::tell)
        .instanceFunction("sendText", &PlayerClass::tell)
        .instanceFunction("getHand", &PlayerClass::getHand)
        .instanceFunction("getAllItems", &PlayerClass::getAllItems)
        .instanceFunction("rename", &PlayerClass::rename)
        .instanceFunction("addLevel", &PlayerClass::addLevel)
        .instanceFunction("setOnFire", &PlayerClass::setOnFire)
        .instanceFunction("transServer", &PlayerClass::transServer)
        .instanceFunction("crash", &PlayerClass::crash)
        .instanceFunction("setOnFire", &PlayerClass::setOnFire)
        .instanceFunction("getDevice", &PlayerClass::getDevice)
        .instanceFunction("removeItem", &PlayerClass::removeItem)

        .instanceFunction("getScore", &PlayerClass::getScore)
        .instanceFunction("setScore", &PlayerClass::setScore)
        .instanceFunction("addScore", &PlayerClass::addScore)
        .instanceFunction("removeScore", &PlayerClass::removeScore)
        .instanceFunction("setSidebar", &PlayerClass::setSidebar)
        .instanceFunction("removeSidebar", &PlayerClass::removeSidebar)
        .instanceFunction("setBossBar", &PlayerClass::setBossBar)
        .instanceFunction("removeBossBar", &PlayerClass::removeBossBar)

        .instanceFunction("sendSimpleForm", &PlayerClass::sendSimpleForm)
        .instanceFunction("sendModalForm", &PlayerClass::sendModalForm)
        .instanceFunction("sendCustomForm", &PlayerClass::sendCustomForm)
        .instanceFunction("sendForm", &PlayerClass::sendForm)

        .instanceFunction("setExtraData", &PlayerClass::setExtraData)
        .instanceFunction("getExtraData", &PlayerClass::getExtraData)
        .instanceFunction("delExtraData", &PlayerClass::delExtraData)

        .instanceFunction("setTag", &PlayerClass::setTag)
        .instanceFunction("getTag", &PlayerClass::getTag)
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
        string target = args[0].toStr();
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

Local<Value> Broadcast(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)

    try {
        TextType type = TextType::RAW;
        if (args.size() >= 2 && args[1].isNumber())
        {
            int newType = args[1].asNumber().toInt32();
            if (newType >= 0 && newType <= 9)
                type = (TextType)newType;
        }
        return Boolean::newBoolean(Raw_Broadcast(args[0].toStr(), type));
    }
    CATCH("Fail in Broadcast!")
}

//成员函数
void PlayerClass::set(Player* player)
{
    __try
    {
        id = ((Actor*)player)->getUniqueID();
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        isValid = false;
    }
}

Player* PlayerClass::get()
{
    if (!isValid)
        return nullptr;
    else
        return Raw_GetPlayerByUniqueId(id);
}

Local<Value> PlayerClass::getName()
{ 
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return String::newString(Raw_GetPlayerName(player));
    }
    CATCH("Fail in getPlayerName!")
}

Local<Value> PlayerClass::getPos()
{ 
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return FloatPos::newPos(Raw_GetPlayerPos(player));
    }
    CATCH("Fail in getPlayerPos!")
}

Local<Value> PlayerClass::getXuid()
{
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return String::newString(Raw_GetXuid(player));
    }
    CATCH("Fail in getXuid!")
}

Local<Value> PlayerClass::getUuid()
{
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return String::newString(Raw_GetUuid(player));
    }
    CATCH("Fail in getXuid!")
}

Local<Value> PlayerClass::getRealName()
{
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return String::newString(Raw_GetRealName(player));
    }
    CATCH("Fail in getRealName!")
}

Local<Value> PlayerClass::getIP()
{
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return String::newString(Raw_GetIP(player));
    }
    CATCH("Fail in GetIP!")
}

Local<Value> PlayerClass::getPermLevel()
{
    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Number::newNumber(Raw_GetPlayerPermLevel(player));
    }
    CATCH("Fail in getPlayerPermLevel!")
}

Local<Value> PlayerClass::getGameMode()
{
    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Number::newNumber(Raw_GetGameMode(player));
    }
    CATCH("Fail in getGameMode!")
}

Local<Value> PlayerClass::getSneaking()
{
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Boolean::newBoolean(Raw_GetSneaking(player));
    }
    CATCH("Fail in getSneaking!")
}

Local<Value> PlayerClass::getSpeed()
{
    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Number::newNumber(Raw_GetSpeed((Actor*)player));
    }
    CATCH("Fail in getSpeed!")
}

Local<Value> PlayerClass::getDirection()
{
    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Number::newNumber(Raw_GetDirection(player));
    }
    CATCH("Fail in getSpeed!")
}

Local<Value> PlayerClass::getMaxHealth()
{
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Number::newNumber(Raw_GetMaxHealth((Actor*)player));
    }
    CATCH("Fail in GetMaxHealth!")
}

Local<Value> PlayerClass::getHealth()
{
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Number::newNumber(Raw_GetHealth((Actor*)player));
    }
    CATCH("Fail in GetHealth!")
}

Local<Value> PlayerClass::getInAir()
{
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Boolean::newBoolean(Raw_GetIsInAir((Actor*)player));
    }
    CATCH("Fail in GetInAir!")
}

Local<Value> PlayerClass::getRawPtr(const Arguments& args)
{
    try {
        Player* player = get();
        if (!player)
            return Local<Value>();
        else
            return Number::newNumber((intptr_t)player);
    }
    CATCH("Fail in getRawPtr!")
}

Local<Value> PlayerClass::teleport(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        FloatVec4 pos;

        if (args.size() == 1)
        {
            // FloatPos
            FloatPos* posObj = FloatPos::extractPos(args[0]);
            if (posObj)
            {
                pos = *posObj;
            }
            else
            {
                ERROR("Wrong type of argument in teleport!");
                return Local<Value>();
            }
        }
        else if (args.size() == 4)
        {
            // number pos
            CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[1], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[2], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[3], ValueKind::kNumber);

            pos.x = args[0].asNumber().toFloat();
            pos.y = args[1].asNumber().toFloat();
            pos.z = args[2].asNumber().toFloat();
            pos.dim = args[3].toInt();
        }
        else
        {
            ERROR("Wrong type of argument in teleport!");
            return Local<Value>();
        }

        Player* player = get();
        if (!player)
            return Local<Value>();
        return Boolean::newBoolean(Raw_TeleportPlayer(player, pos));
    }
    CATCH("Fail in TeleportPlayer!")
}

Local<Value> PlayerClass::kill(const Arguments& args)
{
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Boolean::newBoolean(Raw_KillPlayer(player));
    }
    CATCH("Fail in KillPlayer!")
}

Local<Value> PlayerClass::isOP(const Arguments& args)
{
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Boolean::newBoolean(Raw_GetPlayerPermLevel(player) >= 1);
    }
    CATCH("Fail in IsOP!")
}

Local<Value> PlayerClass::setPermLevel(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kNumber)
    
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        bool res = false;
        int newPerm = args[0].asNumber().toInt32();
        if(newPerm>=0 || newPerm<=4)
            res = Raw_SetPlayerPermLevel(player,newPerm);
        return Boolean::newBoolean(res);
    }
    CATCH("Fail in setPlayerPermLevel!")
}

Local<Value> PlayerClass::setGameMode(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber)

    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        bool res = false;
        int newMode = args[0].asNumber().toInt32();
        if (newMode >= 0 || newMode <= 3)
            res = Raw_SetGameMode(player, newMode);
        return Boolean::newBoolean(res);
    }
    CATCH("Fail in setGameMode!")
}

Local<Value> PlayerClass::runcmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        string cmd{ args[0].toStr() };
        if (cmd[0] == '/')
            cmd.erase(0, 1);
        return Boolean::newBoolean(Raw_RuncmdAs(player,cmd));
    }
    CATCH("Fail in runcmd!")
}

Local<Value> PlayerClass::kick(const Arguments& args)
{
    if(args.size() >= 1)
        CHECK_ARG_TYPE(args[0],ValueKind::kString);

    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        string msg="正在从服务器断开连接";
        if(args.size() >= 1)
            msg = args[0].toStr();
        
        return Boolean::newBoolean(Raw_KickPlayer(player,msg));
    }
    CATCH("Fail in kickPlayer!")
}

Local<Value> PlayerClass::tell(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        TextType type = TextType::RAW;
        if(args.size() >= 2 && args[1].isNumber())
        {
            int newType = args[1].asNumber().toInt32();
            if(newType >= 0 && newType <= 9)
                type = (TextType)newType;
        }
        return Boolean::newBoolean(Raw_Tell(player,args[0].toStr(),type));
    }
    CATCH("Fail in tell!")
}

Local<Value> PlayerClass::getHand(const Arguments& args)
{
    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        return ItemClass::newItem(Raw_GetHand(player));
    }
    CATCH("Fail in getHand!")
}

Local<Value> PlayerClass::getAllItems(const Arguments& args)
{
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        ItemStack* hand;
        ItemStack* offHand;
        vector<ItemStack*> inventory;
        vector<ItemStack*> armor;
        vector<ItemStack*> endChest;

        if (!Raw_GetAllItems(player, &hand, &offHand, &inventory, &armor, &endChest))
        {
            return Local<Value>();
        }
        Local<Object> result = Object::newObject();
        
        //hand
        result.set("hand", ItemClass::newItem(hand));

        //offHand
        result.set("offHand", ItemClass::newItem(offHand));

        //inventory
        Local<Array> inventoryArr = Array::newArray();
        for(ItemStack* item : inventory)
        {
            inventoryArr.add(ItemClass::newItem(item));
        }
        result.set("inventory", inventoryArr);

        //armor
        Local<Array> armorArr = Array::newArray();
        for (ItemStack* item : armor)
        {
            armorArr.add(ItemClass::newItem(item));
        }
        result.set("armor", armorArr);

        //endChest
        Local<Array> endChestArr = Array::newArray();
        for (ItemStack* item : endChest)
        {
            endChestArr.add(ItemClass::newItem(item));
        }
        result.set("endChest", endChestArr);

        return result;
    }
    CATCH("Fail in getAllItems!")
}

Local<Value> PlayerClass::rename(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Boolean::newBoolean(Raw_RenamePlayer(player,args[0].toStr()));
    }
    CATCH("Fail in RenamePlayer!")
}

Local<Value> PlayerClass::addLevel(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);

    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Boolean::newBoolean(Raw_AddLevel(player, args[0].toInt()));
    }
    CATCH("Fail in addLevel!")
}

Local<Value> PlayerClass::transServer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber)

    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Boolean::newBoolean(Raw_TransServer(player, args[0].toStr(), (short) args[1].toInt()));
    }
    CATCH("Fail in transServer!")
}

Local<Value> PlayerClass::crash(const Arguments& args)
{
    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Boolean::newBoolean(Raw_CrashPlayer(player));
    }
    CATCH("Fail in crashPlayer!")
}

Local<Value> PlayerClass::getDevice(const Arguments& args)
{
    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        return DeviceClass::newDevice(player);
    }
    CATCH("Fail in getDevice!")
}

Local<Value> PlayerClass::removeItem(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        int inventoryId = args[0].toInt();
        int count = args[1].toInt();

        bool result = Raw_RemoveItem(player, inventoryId, count);
        return Boolean::newBoolean(result);
    }
    CATCH("Fail in removeItem!")
}

Local<Value> PlayerClass::getScore(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Number::newNumber(Raw_GetScore(player,args[0].toStr()));
    }
    CATCH("Fail in getScore!")
}

Local<Value> PlayerClass::setScore(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)
    
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Boolean::newBoolean(Raw_SetScore(player,args[0].toStr(),args[1].toInt()));
    }
    CATCH("Fail in getScore!")
}

Local<Value> PlayerClass::addScore(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)
    
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Boolean::newBoolean(Raw_AddScore(player,args[0].toStr(),args[1].toInt()));
    }
    CATCH("Fail in addScore!")
}

Local<Value> PlayerClass::removeScore(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Boolean::newBoolean(Raw_RemoveScore(player,args[0].toStr()));
    }
    CATCH("Fail in removeScore!")
}

Local<Value> PlayerClass::setSidebar(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kObject)
    
    try
    {
        Player* player = get();
        if (!player)
            return Local<Value>();

        std::vector<std::pair<std::string, int>> data;
        auto source = args[1].asObject();
        auto keys = source.getKeyNames();
        for (auto& key : keys)
        {
            data.push_back(make_pair(key, source.get(key).toInt()));
        }

        return Boolean::newBoolean(Raw_SetSidebar(player,args[0].toStr(),data));
    }
    CATCH("Fail in setSidebar!")
}

Local<Value> PlayerClass::removeSidebar(const Arguments& args)
{
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Boolean::newBoolean(Raw_RemoveSidebar(player));
    }
    CATCH("Fail in removeSidebar!")
}

Local<Value> PlayerClass::setBossBar(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)
    
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        int percent = args[1].toInt();
        if(percent < 0)
            percent = 0;
        else if(percent > 100)
            percent = 100;
        
        float value = (float)percent / 100;
        return Boolean::newBoolean(Raw_SetBossBar(player,args[0].toStr(),value));
    }
    CATCH("Fail in setBossBar!")
}

Local<Value> PlayerClass::removeBossBar(const Arguments& args)
{
    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        return Boolean::newBoolean(Raw_RemoveBossBar(player));
    }
    CATCH("Fail in removeBossBar!")
}

Local<Value> PlayerClass::sendSimpleForm(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,4)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    CHECK_ARG_TYPE(args[2],ValueKind::kArray)
    CHECK_ARG_TYPE(args[3], ValueKind::kArray)
    CHECK_ARG_TYPE(args[4],ValueKind::kFunction)

    try
    {   
        Player* player = get();
        if (!player)
            return Local<Value>();

        // 普通格式
        auto textsArr = args[2].asArray();
        if (textsArr.size() == 0 || !textsArr.get(0).isString())
            return Local<Value>();
        auto imagesArr = args[3].asArray();
        if (imagesArr.size() != textsArr.size() || !imagesArr.get(0).isString())
            return Local<Value>();

        vector<string> texts, images;
        for (int i = 0; i < textsArr.size(); ++i)
        {
            texts.push_back(textsArr.get(i).toStr());
            images.push_back(imagesArr.get(i).toStr());
        }

        int formId = Raw_SendSimpleForm(player, args[0].toStr(), args[1].toStr(), texts, images);

        
        ENGINE_OWN_DATA()->formCallbacks[(unsigned)formId].engine = EngineScope::currentEngine();
        ENGINE_OWN_DATA()->formCallbacks[(unsigned)formId].func = args[4].asFunction();

        return Number::newNumber(formId);
    }
    CATCH("Fail in sendSimpleForm!")
}

Local<Value> PlayerClass::sendModalForm(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 5)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)
    CHECK_ARG_TYPE(args[1], ValueKind::kString)
    CHECK_ARG_TYPE(args[2], ValueKind::kString)
    CHECK_ARG_TYPE(args[3], ValueKind::kString)
    CHECK_ARG_TYPE(args[4], ValueKind::kFunction)

    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        int formId = Raw_SendModalForm(player, args[0].toStr(), args[1].toStr(), args[2].toStr(), args[3].toStr());
        ENGINE_OWN_DATA()->formCallbacks[(unsigned)formId].engine = EngineScope::currentEngine();
        ENGINE_OWN_DATA()->formCallbacks[(unsigned)formId].func = args[4].asFunction();

        return Number::newNumber(formId);
    }
    CATCH("Fail in sendModalForm!")
}

Local<Value> PlayerClass::sendCustomForm(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kFunction)

    try{
        Player* player = get();
        if (!player)
            return Local<Value>();

        string data = JSON_VALUE::parse(args[0].toStr()).dump();
        int formId = Raw_SendCustomForm(player, data);
        
        ENGINE_OWN_DATA()->formCallbacks[(unsigned)formId].engine = EngineScope::currentEngine();
        ENGINE_OWN_DATA()->formCallbacks[(unsigned)formId].func = args[1].asFunction();
        
        return Number::newNumber(formId);
    }
    catch (const JSON_VALUE::exception& e)
    {
        ERROR("Fail to parse Json string in sendCustomForm!");
        ERRPRINT(e.what());

        return Local<Value>();
    }
    CATCH("Fail in sendCustomForm!")
}

Local<Value> PlayerClass::sendForm(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2)
    CHECK_ARG_TYPE(args[1], ValueKind::kFunction)

    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        int formId = 0;
        auto jsonForm = SimpleFormClass::extractForm(args[0]);
        if(jsonForm == nullptr)
            jsonForm = CustomFormClass::extractForm(args[0]);

        if (jsonForm != nullptr)
        {
            int formId = Raw_SendRawForm(player, jsonForm->dump());
            ENGINE_OWN_DATA()->formCallbacks[(unsigned)formId] = { EngineScope::currentEngine(),Global<Function>(args[1].asFunction()) };

            return Number::newNumber(formId);
        }
        else
        {
            ERROR("Unknown Type of Form Parameter!");
            return Local<Value>();
        }
    }
    catch (const JSON_VALUE::exception& e)
    {
        ERROR("Fail to parse Json string in sendForm!");
        ERRPRINT(e.what());

        return Local<Value>();
    }
    CATCH("Fail in sendForm!")
}

Local<Value> PlayerClass::setExtraData(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try
    {
        Player* player = get();
        if (!player)
            return Local<Value>();

        string key = args[0].toStr();
        if(key.empty())
            return Boolean::newBoolean(false);
    
        ENGINE_OWN_DATA()->playerDataDB[Raw_GetPlayerName(player) + "-" + key] = args[1];
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in setExtraData!")
}

Local<Value> PlayerClass::getExtraData(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try
    {
        Player* player = get();
        if (!player)
            return Local<Value>();

        string key = args[0].toStr();
        if (key.empty())
            return Boolean::newBoolean(false);

        return ENGINE_OWN_DATA()->playerDataDB.at(Raw_GetPlayerName(player) + "-" + key).get();
    }
    catch (const std::out_of_range& e)
    {
        return Local<Value>();
    }
    CATCH("Fail in getExtraData!")
}

Local<Value> PlayerClass::delExtraData(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try
    {
        Player* player = get();
        if (!player)
            return Local<Value>();

        string key = args[0].toStr();
        if(key.empty())
            return Boolean::newBoolean(false);
    
        ENGINE_OWN_DATA()->playerDataDB.erase(Raw_GetPlayerName(player) + "-" + key);
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in delExtraData!")
}

Local<Value> PlayerClass::setOnFire(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);

    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        int time = args[0].toInt();
        bool result = Raw_SetOnFire((Actor*)player, time);
        return Boolean::newBoolean(result);
    }
    CATCH("Fail in setOnFire!")
}

Local<Value> PlayerClass::getTag(const Arguments& args)
{
    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        return NbtCompound::newNBT(Tag::fromActor((Actor*)player));
    }
    CATCH("Fail in getTag!")
}

Local<Value> PlayerClass::setTag(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);

    try {
        Player* player = get();
        if (!player)
            return Local<Value>();

        auto nbt = NbtCompound::extractNBT(args[0]);
        if (!nbt)
            return Local<Value>();    //Null

        nbt->setActor((Actor*)player);
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in setTag!")
}