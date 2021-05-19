#include "Block.h"
using namespace std;

string Raw_GetBlockName(Block* block)
{
    void* hashedstr = SymCall("?getName@Block@@QEBAAEBVHashedString@@XZ",
        void*, void*)(block);
    auto blockname = ((const char*)hashedstr + 8);
    
    return string(blockname);
}