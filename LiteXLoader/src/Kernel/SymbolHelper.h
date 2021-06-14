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
    MCINLINE ScoreInfo getPlayerScore(ScoreboardId const& arg0) {
        ScoreInfo(Objective:: * fnp)(ScoreboardId const&)const;
        *((void**)&fnp) = dlsym("?getPlayerScore@Objective@@QEBA?AUScoreInfo@@AEBUScoreboardId@@@Z");
        return (this->*fnp)(arg0);
    }
};

class Scoreboard
{
public:
    MCINLINE Objective* getObjective(std::string const& arg0) {
        Objective* (Scoreboard:: * fnp)(std::string const&)const;
        *((void**)&fnp) = dlsym("?getObjective@Scoreboard@@QEBAPEAVObjective@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z");
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

struct ScorePacketInfo
{
    ScoreboardId sid;
    string obj_name = "name";
    unsigned score;
    enum Type : char { Invalid = 0, Player = 1, Actor = 2, Fake = 3 };
    Type type = Fake;
    void* pid;
    void* aid;
    string fake_name;

    ScorePacketInfo(const ScoreboardId &s, unsigned num, const string& fake)
        : sid(s), score(num), fake_name(fake)
    { }
};

//全局变量
extern Minecraft* mc;
extern Scoreboard* g_scoreboard;

//辅助函数
BlockSource* Raw_GetBlockSourceByActor(Actor* actor);
int Raw_GetBlockDimension(BlockSource* bs);
Block* Raw_GetBlockByPos(IntVec4* pos);
Block* Raw_GetBlockByPos(int x, int y, int z, BlockSource* bs);