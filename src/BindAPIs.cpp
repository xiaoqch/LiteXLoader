#include <list>
// Ignore error below
#include "ScriptX.h"

#include "API/BaseAPI.h"
#include "API/ActorAPI.h"
#include "API/BlockAPI.h"
#include "API/ItemAPI.h"
#include "API/EventAPI.h"
#include "API/NbtAPI.h"
#include "API/GuiAPI.h"
#include "API/LogAPI.h"
#include "API/DbAPI.h"
#include "API/FileSystemAPI.h"
#include "API/NetworkAPI.h"

void BindAPIs(std::shared_ptr<script::ScriptEngine> engine)
{
    //////////////// BaseAPI ////////////////

    engine->set("runCmd",script::Function::newFunction(RunCmd));

    //////////////// EventAPI ////////////////

    InitEventAPIs();

    auto funcListen = script::Function::newFunction(AddEventListener);
    engine->set("listen", funcListen);
    engine->set("addEventListener", funcListen);


    //////////////// LogAPI ////////////////

    auto funcPrint = script::Function::newFunction(Print);
    engine->set("print", funcPrint);
	engine->set("log", funcPrint);
}