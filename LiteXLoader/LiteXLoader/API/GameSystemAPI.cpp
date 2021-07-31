#include "GameSystemAPI.h"
#include "APIHelp.h"
#include <Kernel/SymbolHelper.h>

//////////////////// APIs ////////////////////

Local<Value> NewScoreObjective(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)
    if(args.size()>=2)
        CHECK_ARG_TYPE(args[1], ValueKind::kString)

    try {
        string name = args[0].toStr();
        string display = name;
        if (args.size() >= 2)
            display = args[1].toStr();

        string criteria = "dummy";
        globalScoreBoard->addObjective(name, display, globalScoreBoard->getCriteria(criteria));

        return Boolean::newBoolean(true);
    }
    CATCH("Fail in NewScoreObjective!")
}

Local<Value> RemoveScoreObjective(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)

        try {
            string name = args[0].toStr();

            auto objs = globalScoreBoard->getObjectiveNames();
            for (auto& obj : objs)
            {
                if (obj == name)
                {
                    globalScoreBoard->removeObjective(globalScoreBoard->getObjective(name));
                    return Boolean::newBoolean(true);
                }
            }

            return Boolean::newBoolean(false);
        }
    CATCH("Fail in RemoveScoreObjective!")
}

Local<Value> ListAllScoreObjective(const Arguments& args)
{
    try {
        Local<Object> res = Object::newObject();

        auto objs = globalScoreBoard->getObjectiveNames();
        for (auto& obj : objs)
        {
            Objective * item = globalScoreBoard->getObjective(obj);
            res.set("name", String::newString(item->getName()));
            res.set("displayName", String::newString(item->getDisplayName()));
        }

        return res;
    }
    CATCH("Fail in ListAllScoreObjective!")
}