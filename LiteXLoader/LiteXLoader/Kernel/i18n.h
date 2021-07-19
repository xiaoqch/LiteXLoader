#pragma once
#include <string>

struct LangPack;
extern LangPack LangP;

void InitI18n(const std::string& langPack);

#ifndef _TRS
	#define _TRS(x) (LangP.trans<do_hash((x))>(x))
#endif

#ifndef _TR
	#define _TR(x) (LangP.trans<do_hash((x))>(x).c_str())
#endif