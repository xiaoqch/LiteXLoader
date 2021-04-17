#include "pch.h"
#include "ScriptX.h"

std::shared_ptr<script::ScriptEngine> engine;
void InitEngine()
{
#ifdef SCRIPTX_BACKEND_V8
	//v8::V8::InitializeExternalStartupData(".");
#endif

#if !defined(SCRIPTX_BACKEND_WEBASSEMBLY)
	engine = std::shared_ptr<script::ScriptEngine>{
		new script::ScriptEngineImpl(),
			script::ScriptEngine::Deleter()
	};
#else
	engine = std::shared_ptr<script::ScriptEngine>{
		script::ScriptEngineImpl::instance(),
			[](void*) {}
	};
#endif
}

std::string_view GetScript() {
	using std::string_view_literals::operator""sv;
#ifdef SCRIPTX_LANG_JAVASCRIPT
	return R"(
console.log("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
)"sv;
#elif defined(SCRIPTX_LANG_LUA)
	return R"(
print("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb")
)"sv;

#else
	throw std::logic_error("Unsupported script language");
#endif
}

void entry() {
    InitEngine();
    script::EngineScope enter(engine.get());
    try {
        engine->eval(GetScript());
    }
    catch (const script::Exception& e) {
        std::cerr << "failed to run game " << e;
    }
}