#include "pch.h"

using EnginePtr = std::shared_ptr<script::ScriptEngine>;
std::list<EnginePtr> modules;
void LoadScript(const std::string& path);
void BindAPIs(EnginePtr engine);
extern std::string_view GetScriptBaseLib();

void entry()
{
	INFO(std::string("LiteXLoader Script Plugin Loader for ") + LXL_SCRIPT_LANG_TYPE +" - Based on LiteLoader");
	INFO(std::string("Version ") + LXL_VERSION);
	std::filesystem::directory_iterator files(conf::getString("Main","PluginsDir",LXL_DEF_LOAD_PATH));
	INFO("Config files loaded.");

	INFO("Loading plugins...");
	for (auto& i : files) {
		if (i.is_regular_file() && i.path().extension() == LXL_PLUGINS_SUFFIX)
		{
			try
			{
				LoadScript(i.path().string());
				INFO(i.path().filename().string() + " loaded.");
			}
			catch(script::Exception& e)
			{
				script::EngineScope enter(modules.back().get());
				ERROR("Fail to load " + i.path().filename().string() + "!\n");
				std::cerr << e << std::endl;
			}
			catch(std::exception& e)
			{
				ERROR("Fail to load " + i.path().filename().string() + "!");
				ERROR(e.what());
			}
			catch(...)
			{
				ERROR("Fail to load " + i.path().filename().string() + "!");
			}
		}
	}
	//Event::
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