#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;
#include <list>

//////////////////// Classes ////////////////////
enum class WSClientEvents : char
{
    onTextReceived = 0, onBinaryReceived, onError, onLostConnection,
    EVENT_COUNT
};

struct ListenerListType
{
    ScriptEngine* engine;
    Global<Function> func;
};

class WSClientClass : public ScriptClass
{
private:
    WebSocketClient ws;
    std::list<ListenerListType> listeners[(int)WSClientEvents::EVENT_COUNT];
    void addListener(const string& event, Local<Function> func);

public:
    explicit WSClientClass();
    ~WSClientClass() { ws.Shutdown(); }

    static Local<Object> newWSClient();

    Local<Value> getStatus();

    Local<Value> connect(const Arguments& args);
    Local<Value> send(const Arguments& args);
    Local<Value> listen(const Arguments& args);
    Local<Value> close(const Arguments& args);
    Local<Value> shutdown(const Arguments& args);
    Local<Value> errorCode(const Arguments& args);
};


//////////////////// APIs ////////////////////

Local<Value> NewWebSocket(const Arguments& args);
Local<Value> HttpGet(const Arguments& args);
Local<Value> HttpPost(const Arguments& args);
Local<Value> HttpGetSync(const Arguments& args);