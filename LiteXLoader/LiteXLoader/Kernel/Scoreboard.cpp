#include "Scoreboard.h"
#include "SymbolHelper.h"
#define H do_hash
#define scb globalScoreBoard

bool checkSlotName(const std::string& slot)
{
	switch (H(slot.c_str()))
	{
	case H("sidebar"): break;
	case H("list"): break;
	case H("belowname"): break;
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
bool Raw_ModifyScoreInObjective(const std::string& objname, const std::string& id, char mode, int score)
{
	auto obj = scb->getObjective(objname);
	auto identity = scb->getScoreboardId(id);
	return scb->getScoreboardIdentityRef(&identity)->modifyScoreInObjective(0, obj, score, (PlayerScoreSetFunction)mode);
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
	return obj->getPlayerScore(identity).getCount();
}