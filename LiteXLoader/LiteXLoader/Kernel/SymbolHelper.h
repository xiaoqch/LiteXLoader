#include "Global.h"
#include "Scoreboard.h"
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

class Container
{
public:
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
    //bool Container::hasRoomForItem(ItemStack const &)
    MCINLINE bool hasRoomForItem(ItemStack const& arg0) {
        bool (Container:: * fnp)(ItemStack const&)const;
        *((void**)&fnp) = dlsym("?hasRoomForItem@Container@@UEAA_NAEBVItemStack@@@Z");
        return (this->*fnp)(arg0);
    }
    //bool Container::isEmpty()const 
    MCINLINE bool isEmpty() {
        bool (Container:: * fnp)()const;
        *((void**)&fnp) = dlsym("?isEmpty@Container@@UEBA_NXZ");
        return (this->*fnp)();
    }
    //std::vector<ItemStack,std::allocator<ItemStack> > Container::getSlotCopies()const 
    MCINLINE std::vector<ItemStack, std::allocator<ItemStack> > getSlotCopies() {
        std::vector<ItemStack, std::allocator<ItemStack> >(Container:: * fnp)()const;
        *((void**)&fnp) = dlsym("?getSlotCopies@Container@@UEBA?AV?$vector@VItemStack@@V?$allocator@VItemStack@@@std@@@std@@XZ");
        return (this->*fnp)();
    }
    //std::vector<ItemStack const *,std::allocator<ItemStack const *> > const Container::getSlots()const 
    MCINLINE std::vector<ItemStack*, std::allocator<ItemStack *> >  getSlots() {
        std::vector<ItemStack *, std::allocator<ItemStack*> >  (Container:: * fnp)()const;
        *((void**)&fnp) = dlsym("?getSlots@Container@@UEBA?BV?$vector@PEBVItemStack@@V?$allocator@PEBVItemStack@@@std@@@std@@XZ");
        return (this->*fnp)();
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
    string obj_name = "FakeScoreObj";
    unsigned score;
    enum Type : char { Invalid = 0, Player = 1, Actor = 2, Fake = 3 };
    Type type = Fake;
    unsigned long long pid;
    unsigned long long aid;
    string fake_name;

    ScorePacketInfo(ScoreboardId *s, unsigned num, const string& fake)
        : sid(*s), score(num), fake_name(fake)
    { }
};

//全局变量
extern Minecraft* mc;

//辅助函数
BlockSource* Raw_GetBlockSourceByActor(Actor* actor);
BlockSource* Raw_GetBlockSourceByDim(int dimid);
int Raw_GetBlockDimension(BlockSource* bs);
Block* Raw_GetBlockByPos(IntVec4* pos);
Block* Raw_GetBlockByPos(BlockPos* bp, BlockSource* bs);
Block* Raw_GetBlockByPos(BlockPos* bp, int dimid);
Block* Raw_GetBlockByPos(int x, int y, int z, BlockSource* bs);
Actor* Raw_GetFishingHookOwner(FishingHook* fh);