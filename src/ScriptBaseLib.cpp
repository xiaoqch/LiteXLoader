#include <string>
#include "ScriptX.h"

std::string_view GetScriptBaseLib()
{
	using std::string_view_literals::operator""sv;
#ifdef SCRIPTX_LANG_JAVASCRIPT
	return R"(

)"sv;
#elif defined(SCRIPTX_LANG_LUA)
	return R"(

)"sv;
#else
	return "";
#endif
}