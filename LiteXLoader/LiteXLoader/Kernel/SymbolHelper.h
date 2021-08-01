#include "Global.h"
#include <string>

//类定义
class BlockActor
{
public:
    MCINLINE BlockPos const& getPosition() {
        BlockPos const& (BlockActor:: * fnp)()const;
        *((void**)&fnp) = dlsym("?getPosition@BlockActor@@QEBAAEBVBlockPos@@XZ");
        return (this->*fnp)();
    }
};
struct ScoreInfo
{
    int getCount()
    {
        return dAccess<int>(this, 12);
    }
};

struct ScoreboardId
{
    int id;
    void* null;
};

class Objective
{
public:
    //ScoreInfo Objective::getPlayerScore(ScoreboardId const &)const 
    MCINLINE ScoreInfo getPlayerScore(ScoreboardId const& arg0) {
        ScoreInfo(Objective:: * fnp)(ScoreboardId const&)const;
        *((void**)&fnp) = dlsym("?getPlayerScore@Objective@@QEBA?AUScoreInfo@@AEBUScoreboardId@@@Z");
        return (this->*fnp)(arg0);
    }
    //std::basic_string<char,std::char_traits<char>,std::allocator<char> > const & Objective::getDisplayName()const 
    MCINLINE std::string const& getDisplayName() {
        std::string const& (Objective:: * fnp)()const;
        *((void**)&fnp) = dlsym("?getDisplayName@Objective@@QEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ");
        return (this->*fnp)();
    }
    //std::basic_string<char,std::char_traits<char>,std::allocator<char> > const & Objective::getName()const 
    MCINLINE std::string const& getName() {
        std::string const& (Objective:: * fnp)()const;
        *((void**)&fnp) = dlsym("?getName@Objective@@QEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ");
        return (this->*fnp)();
    }
};

class ObjectiveCriteria;

class Scoreboard
{
public:
    MCINLINE Objective* getObjective(std::string const& arg0) {
        Objective* (Scoreboard:: * fnp)(std::string const&)const;
        *((void**)&fnp) = dlsym("?getObjective@Scoreboard@@QEBAPEAVObjective@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z");
        return (this->*fnp)(arg0);
    }

    //std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > Scoreboard::getObjectiveNames()const 
    MCINLINE std::vector<std::string, std::allocator<std::string > > getObjectiveNames() {
        std::vector<std::string, std::allocator<std::string > >(Scoreboard:: * fnp)()const;
        *((void**)&fnp) = dlsym("?getObjectiveNames@Scoreboard@@QEBA?AV?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@XZ");
        return (this->*fnp)();
    }

    //Objective * Scoreboard::addObjective(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,ObjectiveCriteria const &)
    MCINLINE Objective* addObjective(std::string const& arg0, std::string const& arg1, ObjectiveCriteria const* arg2) {
        Objective* (Scoreboard:: * fnp)(std::string const&, std::string const&, ObjectiveCriteria const*)const;
        *((void**)&fnp) = dlsym("?addObjective@Scoreboard@@QEAAPEAVObjective@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@0AEBVObjectiveCriteria@@@Z");
        return (this->*fnp)(arg0, arg1, arg2);
    }

    //bool Scoreboard::removeObjective(Objective *)
    MCINLINE bool removeObjective(Objective* arg0) {
        bool (Scoreboard:: * fnp)(Objective*)const;
        *((void**)&fnp) = dlsym("?removeObjective@Scoreboard@@QEAA_NPEAVObjective@@@Z");
        return (this->*fnp)(arg0);
    }

    //ObjectiveCriteria * Scoreboard::getCriteria(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &)const 
    MCINLINE ObjectiveCriteria* getCriteria(std::string const& arg0) {
        ObjectiveCriteria* (Scoreboard:: * fnp)(std::string const&)const;
        *((void**)&fnp) = dlsym("?getCriteria@Scoreboard@@QEBAPEAVObjectiveCriteria@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z");
        return (this->*fnp)(arg0);
    }

    MCINLINE ScoreboardId const& createScoreboardId(std::string const& arg0) {
        ScoreboardId const& (Scoreboard:: * fnp)(std::string const&)const;
        *((void**)&fnp) = dlsym("?createScoreboardId@Scoreboard@@UEAAAEBUScoreboardId@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z");
        return (this->*fnp)(arg0);
    }

    MCINLINE ScoreboardId const& getScoreboardId(Actor const& arg0) {
        ScoreboardId const& (Scoreboard:: * fnp)(Actor const&)const;
        *((void**)&fnp) = dlsym("?getScoreboardId@Scoreboard@@QEBAAEBUScoreboardId@@AEBVActor@@@Z");
        return (this->*fnp)(arg0);
    }

    MCINLINE int modifyPlayerScore(bool& arg0, ScoreboardId const& arg1, Objective& arg2, int arg3, int arg4) {
        int (Scoreboard:: * fnp)(bool&, ScoreboardId const&, Objective&, int, int)const;
        *((void**)&fnp) = dlsym("?modifyPlayerScore@Scoreboard@@QEAAHAEA_NAEBUScoreboardId@@AEAVObjective@@HW4PlayerScoreSetFunction@@@Z");
        return (this->*fnp)(arg0, arg1, arg2, arg3, arg4);
    }
};

class Container
{
public:
    //std::vector<ItemStack const *,std::allocator<ItemStack const *> > const Container::getSlots()const 
    MCINLINE std::vector<ItemStack const*, std::allocator<ItemStack const*> > const getSlots() {
        std::vector<ItemStack const*, std::allocator<ItemStack const*> > const (Container:: * fnp)()const;
        *((void**)&fnp) = dlsym("?getSlots@Container@@UEBA?BV?$vector@PEBVItemStack@@V?$allocator@PEBVItemStack@@@std@@@std@@XZ");
        return (this->*fnp)();
    }
    //void Container::addItem(ItemStack &)
    MCINLINE void addItem(ItemStack& arg0) {
        void (Container:: * fnp)(ItemStack&)const;
        *((void**)&fnp) = dlsym("?addItem@Container@@UEAAXAEAVItemStack@@@Z");
        return (this->*fnp)(arg0);
    }
    //bool Container::addItemToFirstEmptySlot(ItemStack &)
    MCINLINE bool addItemToFirstEmptySlot(ItemStack& arg0) {
        bool (Container:: * fnp)(ItemStack&)const;
        *((void**)&fnp) = dlsym("?addItemToFirstEmptySlot@Container@@UEAA_NAEAVItemStack@@@Z");
        return (this->*fnp)(arg0);
    }
    //int Container::getItemCount(ItemStack const &)const 
    MCINLINE int getItemCount(ItemStack const& arg0) {
        int (Container:: * fnp)(ItemStack const&)const;
        *((void**)&fnp) = dlsym("?getItemCount@Container@@UEBAHAEBVItemStack@@@Z");
        return (this->*fnp)(arg0);
    }
    //void Container::removeAllItems()
    MCINLINE void removeAllItems() {
        void (Container:: * fnp)()const;
        *((void**)&fnp) = dlsym("?removeAllItems@Container@@UEAAXXZ");
        return (this->*fnp)();
    }
    //void Container::removeItem(int,int)
    MCINLINE void removeItem(int arg0, int arg1) {
        void (Container:: * fnp)(int, int)const;
        *((void**)&fnp) = dlsym("?removeItem@Container@@UEAAXHH@Z");
        return (this->*fnp)(arg0, arg1);
    }
};

struct ScorePacketInfo
{
    ScoreboardId sid;
    string obj_name = "__fake_score_objective__";
    unsigned score;
    enum Type : char { Invalid = 0, Player = 1, Actor = 2, Fake = 3 };
    Type type = Fake;
    unsigned long long pid;
    unsigned long long aid;
    string fake_name;

    ScorePacketInfo(const ScoreboardId &s, unsigned num, const string& fake)
        : sid(s), score(num), fake_name(fake)
    { }
};

//全局变量
extern Minecraft* mc;
extern Scoreboard* globalScoreBoard;

//辅助函数
BlockSource* Raw_GetBlockSourceByActor(Actor* actor);
BlockSource* Raw_GetBlockSourceByDim(int dimid);
int Raw_GetBlockDimension(BlockSource* bs);
Block* Raw_GetBlockByPos(IntVec4* pos);
Block* Raw_GetBlockByPos(BlockPos* bp, BlockSource* bs);
Block* Raw_GetBlockByPos(BlockPos* bp, int dimid);
Block* Raw_GetBlockByPos(int x, int y, int z, BlockSource* bs);
Actor* Raw_GetFishingHookOwner(FishingHook* fh);