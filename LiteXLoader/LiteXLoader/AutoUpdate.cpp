#define LXL_UPDATE_DOMAIN "https://cdn.jsdelivr.net"
#define LXL_UPDATE_INFO_REMOTE "/gh/LiteLDev/update/LXL.json"

#define LXL_UPDATE_PROGRAM "plugins/LiteXLoader/LXLAutoUpdate.dll"
#define LXL_UPDATE_CHECK_PRELOAD "plugins/preload.conf"
#define LXL_UPDATE_CACHE_PATH "plugins/LiteXLoader/Update/"
#define LXL_UPDATE_INFO_RECORD "plugins/LiteXLoader/Update/Update.ini"
//#define LXL_UPDATE_CHECK_INTERVAL 5*60
#define LXL_UPDATE_CHECK_INTERVAL 30

#include <string>
#include <thread>
#include <chrono>
#include <filesystem>
#include <Configs.h>
#include <Kernel/Global.h>
#include <Kernel/ThirdParty.h>
#include <Kernel/Utils.h>
#include <Kernel/Data.h>
using namespace std;

bool Raw_CheckFileMD5(const string& path, const string &md5) { return true; }

void ProcessUpdateInfo(const string &info)
{
	try
	{
		JSON_ROOT data = JSON_ROOT::parse(info);

		if (data["AutoUpdate"].is_object())
		{
			//Get Version
			string verRemote = data["AutoUpdate"]["Version"].get<string>();
			auto versRemote = SplitStrWithPattern(verRemote, ".");
			if (versRemote.size() < 3)
				return;
			int a = stoi(versRemote[0]);
			int b = stoi(versRemote[1]);
			int c = stoi(versRemote[2]);

			if (a == LXL_VERSION_MAJOR && b == LXL_VERSION_MINOR && c == LXL_VERSION_REVISION)
				return;

			//Check existing update
			auto ini = Raw_IniOpen(LXL_UPDATE_INFO_RECORD);
			string existing = ini->GetValue("Info", "Version", "");
			if (!existing.empty())
			{
				auto versExisting = SplitStrWithPattern(existing, ".");
				if (a == stoi(versExisting[0]) && b == stoi(versExisting[1]) && c == stoi(versExisting[2]))
					return;
			}
			Raw_IniClose(ini);

			//Downloading
			DEBUG("检测到新版本，启动更新");
			filesystem::remove_all(LXL_UPDATE_CACHE_PATH);
			filesystem::create_directories(LXL_UPDATE_CACHE_PATH);
			
			string url = data["AutoUpdate"]["URL"];
			string domain, path;
			SplitHttpUrl(url, domain, path);

			httplib::Client cli(domain.c_str());
			if (!cli.is_valid())
			{
				DEBUG("更新启动失败！");
				return;
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
					DEBUG("自动更新下载文件失败！错误码：" + to_string(response->status));
					return;
				}

				string install = file["Install"].get<string>();
				string md5 = file["MD5"].get<string>();
				if (!Raw_CheckFileMD5(localPath, md5))
				{
					DEBUG("文件MD5校验失败！自动更新失败！");
					return;
				}

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
		DEBUG(string("LXL版本更新信息解析失败！") + e.what());
	}
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
		httplib::Client cli(LXL_UPDATE_DOMAIN);
		if (!cli.is_valid())
		{
			WARN("LXL自动更新系统启动失败！");
			return ;
		}

		while (true)
		{
			std::this_thread::sleep_for(std::chrono::seconds(LXL_UPDATE_CHECK_INTERVAL));

			auto response = cli.Get(LXL_UPDATE_INFO_REMOTE);
			if (response && response->status == 200)
			{
				DEBUG("LXL自动更新信息获取成功");

				ProcessUpdateInfo(response->body);
			}
		}
	}).detach();
}