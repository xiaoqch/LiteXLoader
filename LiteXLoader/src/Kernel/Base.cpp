#include "Base.h"
#include <iostream>
using namespace std;

bool Raw_Runcmd(const string &cmd)
{
    return liteloader::runcmd(cmd);
}

std::pair<bool, string> Raw_RuncmdEx(const string &cmd)
{
    return liteloader::runcmdEx(cmd);
}

bool Raw_RegisterCmd(const string &cmd, const string &describe, int cmdLevel)
{
    char *desc = new (std::nothrow) char[describe.size()+1];
    if(!desc)
        return false;
    describe.copy(desc, std::string::npos);

    SymCall("?registerCommand@CommandRegistry@@QEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBDW4CommandPermissionLevel@@UCommandFlag@@3@Z",
        void, void*, std::string const&, char const*, char, char, char)
        (CmdRegGlobal, cmd, desc, cmdLevel, 0, 0x40);
    return true;
}

bool Raw_SendCmdOutput(const std::string& output)
{
    SymCall("??$_Insert_string@DU?$char_traits@D@std@@_K@std@@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@0@AEAV10@QEBD_K@Z",
        ostream& , ostream&, const char*, unsigned)
        (cout, output.c_str(), output.size());
}