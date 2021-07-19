#include "Global.h"
#include "Server.h"
#include "Global.h"
using namespace std;

bool Raw_SetServerMotd(const std::string &motd)
{
	if (!mc)
		return false;
	ServerNetworkHandler* snh = mc->getServerNetworkHandler();
	if (!snh)
		return false;

	SymCall("?allowIncomingConnections@ServerNetworkHandler@@QEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_N@Z",
		long long, ServerNetworkHandler*, const string&, bool)(snh, motd, true);
    return true;
}

bool Raw_SetOnlinePlayer(int nowCount, int maxCount)
{
#define _BYTE char
#define _QWORD uintptr_t

    extern Minecraft *mc;
	if (!mc)
		return false;
	ServerNetworkHandler* v1 = mc->getServerNetworkHandler();
	if (!v1)
		return false;

	if (*((_BYTE*)v1 + 584))
	{
		auto v2 = 0;
		auto v8 = *((_QWORD*)v1 + 10);
		auto v9 = *(void(__fastcall**)(__int64, char*, __int64, _QWORD, int, int, char))(*(_QWORD*)v8 + 24i64);
		auto v10 = *((int*)v1 + 180);
		auto v11 = (*(__int64(__fastcall**)(_QWORD))(**((_QWORD**)v1 + 7) + 1136i64))(*((_QWORD*)v1 + 7));
		auto v12 = SymCall("?getGameType@LevelData@@QEBA?AW4GameType@@XZ",
			char, uintptr_t)(v11);
		auto v13 = (*(__int64(__fastcall**)(_QWORD))(**((_QWORD**)v1 + 7) + 1136i64))(*((_QWORD*)v1 + 7));
		auto v14 = SymCall("?getLevelName@LevelData@@QEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
			__int64, uintptr_t)(v13);
		v9(v8, (char*)v1 + 600, v14, v12, nowCount, maxCount, 1);
	}
    return true;
}