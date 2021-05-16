#include "Base.h"
using namespace std;

bool inline Raw_Runcmd(const string &cmd)
{
    return liteloader::runcmd(cmd);
}

bool inline Raw_RuncmdAs(Player *player, const string &cmd)
{
    return liteloader::runcmdAs(player,cmd);
}

std::pair<bool, string> inline Raw_RuncmdEx(const string &cmd)
{
    return liteloader::runcmdEx(cmd);
}

bool Raw_RegisterCmd(const string &cmd, const string &describe, int cmdLevel)
{
///////////////////////////// May Have Better Here ///////////////////////////// 
    char *desc = new (std::nothrow) char[describe.size()+1];
    if(!desc)
        return;
    describe.copy(desc, std::string::npos);

    SymCall("?registerCommand@CommandRegistry@@QEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBDW4CommandPermissionLevel@@UCommandFlag@@3@Z",
        void, void*, std::string const&, char const*, char, char, char)
        (CmdRegGlobal, cmd, desc, cmdLevel, 0, 0x40);
    return true;
}