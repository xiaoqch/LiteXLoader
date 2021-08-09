#define LXL_UPDATE_DOMAIN "https://cdn.jsdelivr.net"
#define LXL_UPDATE_INFO_REMOTE "/gh/LiteLDev/update@latest/LXL.json"

#define LXL_UPDATE_PROGRAM "plugins/LiteXLoader/LXLAutoUpdate.dll"
#define LXL_UPDATE_CHECK_PRELOAD "plugins/preload.conf"
#define LXL_UPDATE_CACHE_PATH "plugins/LiteXLoader/Update/"
#define LXL_UPDATE_INFO_RECORD "plugins/LiteXLoader/Update/Update.ini"
#define LXL_UPDATE_CHECK_INTERVAL 10*60

#include <string>
#include <thread>
#include <chrono>
#include <process.h>
#include <filesystem>
#include <Version.h>
#include <Kernel/Base.h>
#include <Kernel/Global.h>
#include <Kernel/ThirdParty.h>
#include <Kernel/Utils.h>
#include <Kernel/Data.h>
using namespace std;

void SetHttpHeader(httplib::Client* cli)
{
	cli->set_default_headers({
		{ "cache-control", "max-age=0" },
		{ "UserAgent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.131 Safari/537.36 Edg/92.0.902.67" }
	});
}

bool CheckAutoUpdate(bool isUpdateManually)
{
	try
	{
		httplib::Client cli(LXL_UPDATE_DOMAIN);
		SetHttpHeader(&cli);

		//refresh
		cli.Get(LXL_UPDATE_INFO_REMOTE);
		Sleep(1000);

		auto response = cli.Get(LXL_UPDATE_INFO_REMOTE);
		if (!response || response->status != 200)
		{
			if (isUpdateManually)
			{
				PRINT("LXL自动更新信息拉取失败！");
			}
			else
				DEBUG("LXL自动更新信息拉取失败！");
			return false;
		}
		string info{ response->body };

		JSON_ROOT data = JSON_ROOT::parse(info);

		if (data["AutoUpdate"].is_object())
		{
			//Check BDS Version
			string bds = Raw_GetBDSVersion();
			if (bds.front() == 'v')
				bds.erase(0, 1);
			string bdsRemote = data["AutoUpdate"]["BDS"].get<string>();
			if (bds != bdsRemote && !LXL_VERSION_IS_BETA)
			{
				INFO("提示：您的BDS版本和当前主线维护版本不一致，自动更新将不会推送，避免发生版本冲突");
				INFO("如果有需要，请前往LXL相关页面手动更新加载器");
				if (isUpdateManually)
					return false;
				else
					_endthreadex(0);
			}

			//Get Version
			string verRemote = data["AutoUpdate"]["Version"].get<string>();
			auto versRemote = SplitStrWithPattern(verRemote, ".");
			if (versRemote.size() < 3)
			{
				if (isUpdateManually)
				{
					PRINT("版本信息解析失败！格式错误");
				}
				else
					DEBUG("版本信息解析失败！格式错误");
				return false;
			}
				
			int a = stoi(versRemote[0]);
			int b = stoi(versRemote[1]);
			int c = stoi(versRemote[2]);

			if (a <= LXL_VERSION_MAJOR && b <= LXL_VERSION_MINOR && c <= LXL_VERSION_REVISION)
			{
				if (isUpdateManually)
				{
					PRINT("当前版本已是最新版本，无需更新。");
				}
				return true;
			}

			//Check existing update
			auto ini = Raw_IniOpen(LXL_UPDATE_INFO_RECORD);
			string existing = ini->GetValue("Info", "Version", "");
			if (!existing.empty())
			{
				auto versExisting = SplitStrWithPattern(existing, ".");
				if (a == stoi(versExisting[0]) && b == stoi(versExisting[1]) && c == stoi(versExisting[2]))
					return true;
			}
			Raw_IniClose(ini);

			//Downloading
			if (isUpdateManually)
			{
				PRINT("检测到新版本，启动更新");
			}
			else
				DEBUG("检测到新版本，启动更新");
			filesystem::remove_all(LXL_UPDATE_CACHE_PATH);
			filesystem::create_directories(LXL_UPDATE_CACHE_PATH);
			
			string url = data["AutoUpdate"]["URL"];
			string domain, path;
			SplitHttpUrl(url, domain, path);

			httplib::Client cli(domain.c_str());
			SetHttpHeader(&cli);
			if (!cli.is_valid())
			{
				if (isUpdateManually)
				{
					PRINT("更新启动失败！");
				}
				else
					DEBUG("更新启动失败！");
				return false;
			}

			auto iniUpdate = Raw_IniOpen(LXL_UPDATE_INFO_RECORD);
			for (auto& file : data["AutoUpdate"]["Files"])
			{
				string fileName = file["Name"].get<string>();
				string filePath = path + fileName;
				string localPath = LXL_UPDATE_CACHE_PATH + fileName;

				auto response = cli.Get(filePath.c_str());
				if (response && response->status == 200)
				{
					ofstream fout(localPath, ios::binary);
					fout << response->body;
					fout.close();
				}
				else
				{
					if (isUpdateManually)
					{
						PRINT("自动更新下载文件失败！错误码：" + to_string(response->status));
					}
					else
						DEBUG("自动更新下载文件失败！错误码：" + to_string(response->status));
					return true;
				}

				string install = file["Install"].get<string>();
				string md5 = file["MD5"].get<string>();

				iniUpdate->SetValue(fileName.c_str(), "Install", install.c_str());
				iniUpdate->SetValue(fileName.c_str(), "MD5", md5.c_str());
			}

			iniUpdate->SetValue("Info", "Version", verRemote.c_str());
			iniUpdate->SaveFile(LXL_UPDATE_INFO_RECORD);

			INFO("LXL自动更新文件下载完毕。新版本：v"+verRemote);
			INFO("在你下次重启服务器的时候，LXL将自动更新为最新版本");
		}
	}
	catch (JSON_ROOT::exception& e) {
		if (isUpdateManually)
		{
			PRINT(string("LXL版本更新信息解析失败！") + e.what());
		}
		else
			DEBUG(string("LXL版本更新信息解析失败！") + e.what());
	}
	return false;
}

void AddPreload()
{
	fstream fPreload(LXL_UPDATE_CHECK_PRELOAD, ios::out | ios::in | ios::app);
	fPreload.seekg(0, ios::beg);

	string path;
	while (getline(fPreload, path))
	{
		if (path.back() == '\n')
			path.pop_back();
		if (path.back() == '\r')
			path.pop_back();

		if (path == LXL_UPDATE_PROGRAM)
			return;
	}
	fPreload.clear();
	fPreload << "\n" << LXL_UPDATE_PROGRAM << endl;
}

void InitAutoUpdateCheck()
{
	//Check Files
	if (!filesystem::exists(LXL_UPDATE_PROGRAM))
	{
		WARN("LXL自动更新系统未启动！自动更新程序缺失");
		return;
	}
	if (!filesystem::exists(LXL_UPDATE_CACHE_PATH))
	{
		filesystem::create_directories(LXL_UPDATE_CACHE_PATH);
	}
	AddPreload();

	std::thread([]()
	{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::seconds(LXL_UPDATE_CHECK_INTERVAL));
			CheckAutoUpdate(false);
		}
	}).detach();
}