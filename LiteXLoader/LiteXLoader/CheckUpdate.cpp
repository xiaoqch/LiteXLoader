#include "CheckUpdate.h"
#include <Kernel/System.h>
#include <Configs.h>
#include <string>
#include <Kernel/Global.h>
#include <Kernel/ThirdParty.h>

using namespace std;

string GetVersionString()
{
	return to_string(LXL_VERSION_MAJOR) + "." + to_string(LXL_VERSION_MINOR) + "." + to_string(LXL_VERSION_REVISION);
}

void ProcessData(int code, string body)
{
	if (code < 0)
	{
		WARN("LXL获取版本更新信息失败。");
	}
	else if (code != 200)
	{
		WARN("LXL获取版本更新信息失败。错误码："+to_string(code));
	}
	else
	{
		try
		{
			JSON_ROOT data = JSON_ROOT::parse(body);

			//公告信息
			if (data["notice"].is_array())
			{
				for (auto& element : data["notice"])
					INFO(element.get<string>());
			}

			//版本检查
			if (data["versions"].is_object())
			{
				string ver = GetVersionString();
				for (auto& element : data["versions"].items())
				{
					if (element.key() == ver && element.value().is_object() && (element.value())["notice"].is_array())
					{
						for (auto& info : (element.value())["notice"])
							INFO(info.get<string>());
						break;
					}
				}
			}
			else
				WARN("LXL版本更新信息解析失败。未获取到版本信息");
		}
		catch (JSON_ROOT::exception& e) {
			WARN(string("LXL版本更新信息解析失败。")+e.what());
		}
	}
}

void CheckUpdate()
{
	if (!Raw_HttpGet(LXL_UPDATE_CHECK, ProcessData))
	{
		WARN("LXL获取版本更新信息失败。");
	}
}