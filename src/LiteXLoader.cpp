#include <windows.h>
#include <iostream>
#include <string>
#include <list>
#include <filesystem>
#include <fstream>
#include <exception>
// Ignore error below
#include "ScriptX.h"
#include "ConfigHelp.h"


#define LXL_DEF_LOAD_PATH "./plugins"
#ifdef SCRIPTX_LANG_JAVASCRIPT
    #define PLUGINS_SUFFIX ".js"
#elif defined(SCRIPTX_LANG_LUA)
    #define PLUGINS_SUFFIX ".lua"
#endif

using EnginePtr = std::shared_ptr<script::ScriptEngine>;
std::list<EnginePtr> modules;
void LoadScript(const std::string& path);
void BindAPIs(EnginePtr engine);
extern std::string_view GetScriptBaseLib();

void entry()
{
	std::filesystem::directory_iterator files(conf::getStr("Main","PluginsDir",LXL_DEF_LOAD_PATH));
	std::cout << "[LiteXLoader][Info] Config files loaded." << std::endl;

	std::cout << "[LiteXLoader][Info] Loading plugins..." << std::endl;
	for (auto& i : files) {
		if (i.is_regular_file() && i.path().extension() == PLUGINS_SUFFIX)
		{
			try
			{
				LoadScript(i.path().string());
				std::cout << "[LiteXLoader][Info] " << i.path().filename() << " loaded." << std::endl;
			}
			catch(script::Exception& e)
			{
				std::cout << "[LiteXLoader][Error] Fail to load" << i.path().filename() << "!" << std::endl;
				std::cout << "[LiteXLoader][Error] " << e << std::endl;
			}
			catch(std::exception& e)
			{
				std::cout << "[LiteXLoader][Error] Fail to load" << i.path().filename() << "!" << std::endl;
				std::cout << "[LiteXLoader][Error] " << e.what() << std::endl;
			}
			catch(...)
			{
				std::cout << "[LiteXLoader][Error] Fail to load" << i.path().filename() << "!" << std::endl;

			}
		}
	}
}


/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////

EnginePtr NewEngine()
{
	#if !defined(SCRIPTX_BACKEND_WEBASSEMBLY)
		return EnginePtr{
			new script::ScriptEngineImpl(),
				script::ScriptEngine::Deleter()
		};
	#else
		return EnginePtr{
			script::ScriptEngineImpl::instance(),
				[](void*) {}
		};
	#endif
}

//////// test ////////
void clog(std::string str)
{
	std::cout << str << std::endl;
}

void BindAPIs(EnginePtr engine)
{
	auto logFunc = script::Function::newFunction(clog);
	engine->set("log", logFunc);
}
//////// test ////////

void LoadScript(const std::string& path)
{
	EnginePtr engine = NewEngine();
	modules.push_back(engine);

	script::EngineScope enter(engine.get());
	BindAPIs(engine);

	engine->eval(GetScriptBaseLib());
	std::ifstream fin(path);
	if(!fin.is_open())
	{
		throw std::exception("Fail to open file!");
		return;
	}
	std::string scripts((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
	fin.close();
	engine->eval(scripts);
}