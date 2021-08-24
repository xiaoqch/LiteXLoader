#include "Scoreboard.h"
#include "SymbolHelper.h"
#define H do_hash
#define scb globalScoreBoard

bool checkSlotName(const std::string& slot)
{
	switch (H(slot.c_str()))
	{
	case H(Scoreboard::DISPLAY_SLOT_LIST): break;
	case H(Scoreboard::DISPLAY_SLOT_BELOWNAME): break;
	case H(Scoreboard::DISPLAY_SLOT_SIDEBAR): break;
	default: 
		return false;
	}
	return true;
}

bool Raw_SetDisplayObjective(const std::string& objname, const std::string& slot, int sort)
{
	if (checkSlotName(slot))
	{
		scb->setDisplayObjective(slot, scb->getObjective(objname), (ObjectiveSortOrder)sort);
		return true;
	}
	return false;
}
Objective* Raw_ClearDisplayObjective(const std::string& slot)
{
	if (checkSlotName(slot))
		return scb->clearDisplayObjective(slot);
	return nullptr;
}
Objective* Raw_GetDisplayObjective(const std::string& slot)
{
	if (checkSlotName(slot))
	{
		auto disp = scb->getDisplayObjective(slot);
		if (disp)
			return disp->getObjective();
	}
	return nullptr;
}
int Raw_ModifyScoreInObjective(const std::string& objname, const std::string& id, char mode, int score)
{
	auto obj = scb->getObjective(objname);
	auto identity = scb->getScoreboardId(id);
	if (obj != 0)
	{
		int a1 = 0;
		bool res = scb->getScoreboardIdentityRef(&identity)->
			modifyScoreInObjective(&a1, obj, score, (PlayerScoreSetFunction)mode);
		if (res) return a1;
	}
	return 0;
}
bool Raw_RemoveFromObjective(const std::string& objname, const std::string& id)
{
	auto obj = scb->getObjective(objname);
	auto identity = scb->getScoreboardId(id); 
	return scb->getScoreboardIdentityRef(&identity)->removeFromObjective(scb, obj);
}
int Raw_GetScore(const std::string& objname, const std::string& id)
{
	auto obj = scb->getObjective(objname);
	auto identity = scb->getScoreboardId(id);
	auto scores = scb->getIdScores(identity);
	for (auto& it : scores)
		if (it.getObjective() == obj) 
			return it.getCount();
	return 0;
}