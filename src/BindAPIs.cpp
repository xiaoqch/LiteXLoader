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
    //////////////// EventAPI ////////////////

    // listen("onChat",OnChatFunction)
    auto funcListen = script::Function::newFunction(AddEventListener);
    engine->set("listen", funcListen);
    engine->set("addEventListener", funcListen);


    //////////////// LogAPI ////////////////

    // print("aaa",1,new Array(1,2,3),...)
    auto funcPrint = script::Function::newFunction(Print);
    engine->set("print", funcPrint);
	engine->set("log", funcPrint);
}