#include "APIhelp.h"
#include "NetworkAPI.h"
#include "../Kernel/System.h"
using namespace script;

Local<Value> HttpRequestSync(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,3)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    CHECK_ARG_TYPE(args[2],ValueKind::kString)

    try{
        Local<Object> res = Object::newObject();
        auto response = Raw_HttpRequestSync(args[0].toStr(),args[1].toStr(),args[2].toStr());
        res.set("status",response.first);
        res.set("result",response.second);
        return res;
    }
    CATCH("Fail in HttpRequestSync")
}

Local<Value> HttpRequestAsync(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,3)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    CHECK_ARG_TYPE(args[2],ValueKind::kString)
    CHECK_ARG_TYPE(args[3],ValueKind::kFunction)

    try{
        Global<Function> callbackFunc{args[3].asFunction()};
        return Boolean::newBoolean(Raw_HttpRequestAsync(args[0].toStr(),args[1].toStr(),args[2].toStr(),
            [callback{std::move(callbackFunc)}](int status,string data){
                ////////////////////////////// Engine Scope???? //////////////////////////////
                callback.get().call({},Number::newNumber(status),String::newString(data));
        }));
    }
    CATCH("Fail in HttpRequestAsync")
}