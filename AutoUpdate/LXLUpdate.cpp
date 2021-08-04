#include "pch.h"
#define LXL_UPDATE_CACHE_PATH "plugins/LiteXLoader/Update/"
#define LXL_UPDATE_INFO_RECORD "plugins/LiteXLoader/Update/Update.ini"

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <list>
#include "Hash/md5.h"
#include "SimpleIni.h"
using namespace std;

#define CHECK_MD5_READ_BUF 256*1024
bool Raw_CheckFileMD5(const string& path, const string& md5)
{
	auto calc = Chocobo1::MD5();
	char buff[CHECK_MD5_READ_BUF] = { 0 };

	FILE* fd;
	fopen_s(&fd, path.c_str(), "rb");
	while (!feof(fd))
	{
		int count = fread(buff, sizeof(char), CHECK_MD5_READ_BUF, fd);
		calc.addData(buff, count);
	}
	fclose(fd);

	return calc.finalize().toString() == md5;
}

void LXLUpdate()
{
	if (!filesystem::exists(LXL_UPDATE_INFO_RECORD))
		return;
		
	CSimpleIniA ini;
	ini.SetUnicode(true);
	ini.LoadFile(LXL_UPDATE_INFO_RECORD);

	string newVer(ini.GetValue("Info", "Version", ""));
	if (!newVer.empty())
	{
		cout << "[LiteXLoader][Info] LXL自动更新工作中..." << endl;
		list<CSimpleIniA::Entry> files;
		ini.GetAllSections(files);

		//Check MD5
		for (auto& file : files)
		{
			if (string(file.pItem) == "Info")
				continue;
			string from = string(LXL_UPDATE_CACHE_PATH) + file.pItem;

			if (!Raw_CheckFileMD5(from, ini.GetValue(file.pItem, "MD5","")))
			{
				cout << "[LiteXLoader][Error] MD5校验失败！自动更新未执行。" << endl;
				return;
			}
		}

		//Update
		for (auto& file : files)
		{
			string name(file.pItem);
			if (name == "Info")
				continue;

			string from = LXL_UPDATE_CACHE_PATH + name;
			string to = ini.GetValue(name.c_str(), "Install","") + string("/") + name;

			cout << "[LiteXLoader][Info] 正在更新文件" << to << endl;

			std::error_code ec;
			if (!filesystem::copy_file(from, to, filesystem::copy_options::overwrite_existing, ec))
			{
				cout << "[LiteXLoader][FATAL] 自动更新出现错误！文件复制失败。错误码：" << ec << endl;
				cout << "[LiteXLoader][FATAL] 警告！自动更新不完整可能造成LXL工作异常。建议前往LXL相关链接手动更新加载器" << endl;
			}
		}

		filesystem::remove_all(LXL_UPDATE_CACHE_PATH);
		cout << "[LiteXLoader][Info] LXL自动更新已结束。" << endl;
		cout << "[LiteXLoader][Info] 已更新到最新版本：v" << newVer << endl;
	}
}