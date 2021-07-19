#include "i18n.h"
#include "pch.h"
#include <string>
using namespace std;

LangPack LangP;

void InitI18n(const string &langPack)
{
	LangP.load(langPack);
}