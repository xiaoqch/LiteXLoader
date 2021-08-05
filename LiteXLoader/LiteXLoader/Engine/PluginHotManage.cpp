#include "PluginHotManage.h"
#include <API/APIHelp.h>
#include <Kernel/Utils.h>
#include "LoaderHelper.h"
#include <Configs.h>
#include <string>
using namespace std;

bool ProcessHotManageCmd(const std::string& cmd)
{
    auto cmdList = SplitCmdLine(cmd);

    if (cmdList[0] != LXL_HOT_MANAGE_PREFIX)
        return true;

    if (cmd.find(LXL_HOT_LIST) == 0)
    {
        //list
        auto list = LxlListLocalAllPlugins();
        for (auto& name : list)
            PRINT(name);
    }
    else if (cmd.find(LXL_HOT_LOAD) == 0 && cmdList.size() == 3)
    {
        //load
        if (!filesystem::exists(cmdList[2]))
            ERROR("Plugin no found!");
        if (filesystem::path(cmdList[2]).extension() == LXL_PLUGINS_SUFFIX)
            LxlLoadPlugin(cmdList[2], true);
    }
    else if (cmd.find(LXL_HOT_UNLOAD) == 0 && cmdList.size() == 3)
    {
        //unload
        if (filesystem::path(cmdList[2]).extension() == LXL_PLUGINS_SUFFIX)
            if (LxlUnloadPlugin(cmdList[2]) == "")
                ERROR("Plugin no found!");
    }
    else if (cmd.find(LXL_HOT_RELOAD) == 0)
    {
        if (cmdList.size() == 2)
        {
            //reload all
            LxlReloadAllPlugins();
        }
        else if (cmdList.size() == 3)
        {
            //reload one
            if (filesystem::path(cmdList[2]).extension() == LXL_PLUGINS_SUFFIX)
                if (!LxlReloadPlugin(cmdList[2]))
                    ERROR("Plugin no found!");
        }
        else
            ERROR("Bad Command!");
    }
    else if (cmd.find(LXL_HOT_VERSION) == 0)
    {
        //version
        printf("LiteXLoader v%d.%d.%d%s", LXL_VERSION_MAJOR, LXL_VERSION_MINOR, LXL_VERSION_REVISION);
    }
    else
        ERROR("Bad Command!");
    return false;
}