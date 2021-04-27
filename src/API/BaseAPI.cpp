#include "BaseAPI.h"
#include "APIhelp.h"
#include "PlayerAPI.h"
#include "ItemAPI.h"
#include "BlockAPI.h"
#include "EntityAPI.h"
#include <windows.h>
using namespace script;


//////////////////// APIs ////////////////////

Local<Value> GetName(const Arguments& args)
{ 
    CHECK_ARGS_COUNT(args,1)

    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return String::newString(player->getNameTag());
        
        Actor *entity = ExtractEntity(args[0]);
        if(entity)
            return String::newString(entity->getNameTag());

        Block *block = ExtractBlock(args[0]);
        if(block)
        {
            void* hashedstr = SymCall("?getName@Block@@QEBAAEBVHashedString@@XZ",
                void*, void*)(block);
            auto blockname = ((const char*)hashedstr + 8);
            
            return String::newString(string(blockname));
        }

///////////////////////////// Have better API /////////////////////////////
        ItemStack *item = ExtractItem(args[0]);
        if(item)
            return String::newString(item->getName());
            
        return Local<Value>(); // Null
    }
    CATCH("Fail in GetName!")
}

Local<Value> GetPos(const Arguments& args)
{ 
    CHECK_ARGS_COUNT(args,1)

    try{
        Player *player = ExtractPlayer(args[0]);
        if(player)
            return NewPos(player->getPos(), WPlayer(*player).getDimID());

        Actor *entity = ExtractEntity(args[0]);
        if(entity)
            return NewPos(entity->getPos(), WActor(*entity).getDimID());

///////////////////////////// FIX HERE /////////////////////////////
        /*Block *block = ExtractBlock(args[0]);
        if(block)
        {
            WBlock wp(*block);
            BlockPos bp = wp.getBlockPosition(); 
            
            return NewPos(bp.x, bp.y ,bp.z, wp.getDimID());
        }*/

///////////////////////////// FIX HERE ///////////////////////////// 
        /*ItemStack *item = ExtractItem(args[0]);
        if(item)
            return NewPos(item->getPos(), item->getDimID());*/
            
        return Local<Value>(); // Null
    }
    CATCH("Fail in GetPos!")
}

Local<Value> RunCmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        return Boolean::newBoolean(liteloader::runcmd(args[0].asString().toString()));
    }
    CATCH("Fail in RunCmd!")
}

Local<Value> RunCmdEx(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        std::pair<bool, string> result = liteloader::runcmdEx(args[0].asString().toString());
        Local<Object> resObj = Object::newObject();
        resObj.set("result",result.first);
        resObj.set("output",result.second);
        return resObj;
    }
    CATCH("Fail in RunCmdEx!")
}

Local<Value> RegisterCmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    if(args.size() >= 3)
        CHECK_ARG_TYPE(args[2],ValueKind::kNumber)

    try{
        std::string describe = args[1].asString().toString();
        char *desc = new char[describe.size()+1];
        describe.copy(desc, std::string::npos);
///////////////////////////// May Have Better Here ///////////////////////////// 

        int level = 4;
        if(args.size() >= 3)
        {
            int newLevel = args[2].asNumber().toInt32();
            if(newLevel >= 0 && newLevel <= 4)
                level = newLevel;
        }

        SymCall("?registerCommand@CommandRegistry@@QEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBDW4CommandPermissionLevel@@UCommandFlag@@3@Z",
			void, void*, std::string const&, char const*, char, char, char)
			(CmdRegGlobal, args[0].asString().toString(), desc, level, 0, 0x40);
        
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in RegisterCmd!")
}

Local<Value> Log(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)

    for(int i = 0; i < args.size(); ++i)
        PrintValue(std::cout,args[i]);
    std::cout << std::endl;
    return Boolean::newBoolean(true);
}

Local<Value> GetTimeStr(const Arguments& args)
{
    time_t t = time(NULL);
    tm* ts = localtime(&t);
    char buf[24]= {0};
    strftime(buf, 24, "%Y-%m-%d %H:%M:%S", ts);
    return String::newString(buf);
}

Local<Value> GetTimeNow(const Arguments& args)
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    Local<Object> res = Object::newObject();
    res.set("Y",Number::newNumber((int)st.wYear));
    res.set("M",Number::newNumber((int)st.wMonth));
    res.set("D",Number::newNumber((int)st.wDay));
    res.set("h",Number::newNumber((int)st.wHour));
    res.set("m",Number::newNumber((int)st.wMinute));
    res.set("s",Number::newNumber((int)st.wSecond));
    res.set("ms",Number::newNumber((int)st.wMilliseconds));
    return res;
}

Local<Value> GetLxlVersion(const Arguments& args)
{
    return String::newString(LXL_VERSION);
}