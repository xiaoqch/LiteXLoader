#include "LocalShareData.h"

//DLL本地共享数据
LocalDataType* localShareData;

//命令延迟注册队列
std::vector<RegCmdQueue> toRegCmdQueue;


void InitLocalShareData()
{
	srand(clock());
	localShareData = new LocalDataType;
}