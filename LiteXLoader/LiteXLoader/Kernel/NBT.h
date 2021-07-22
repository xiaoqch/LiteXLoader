#include "Global.h"
#include <string>
#include <map>

enum TagType {
	End, Byte, Short, Int, Int64, Float, Double, ByteArray, String, List, Compound,
};

class Tag {
private:
	char filler[40];
public:
	template<typename T>
	T inline getValue() {
		return *(T*)((uintptr_t)this + 8);
	}

	template<typename T>
	void inline setValue(T v) {
		*(T*)((uintptr_t)this + 8) = v;
	}

	static inline Tag* createTag(TagType t) {
		Tag* tag = 0;
		SymCall("?newTag@Tag@@SA?AV?$unique_ptr@VTag@@U?$default_delete@VTag@@@std@@@std@@W4Type@1@@Z",
			void, Tag**, TagType)(&tag, t);
		return tag;
	}

	char inline getTagType() {
		return (*(__int64(__fastcall**)(const struct Tag*))(*(uintptr_t*)this + 40i64))(this);
	}

	void inline put(string k, Tag* v) {
		return SymCall("?put@CompoundTag@@QEAAAEAVTag@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$$QEAV2@@Z"
			, void, void*, string, Tag*)(this, k, v);
	}

	void inline put(string k, short v) {
		return SymCall("?putShort@CompoundTag@@QEAAAEAFV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@F@Z"
			, void, void*, string, short)(this, k, v);
	}

	void inline put(string k, string v) {
		return SymCall("?putString@CompoundTag@@QEAAAEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V23@0@Z"
			, void, void*, string, string)(this, k, v);
	}

	void inline put(string k, int v) {
		return SymCall("?putInt@CompoundTag@@QEAAAEAHV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@Z"
			, void, void*, string, int)(this, k, v);
	}

	void inline put(string k, __int64 v) {
		return SymCall("?putInt64@CompoundTag@@QEAAAEA_JV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_J@Z",
			void, void*, string, __int64)(this, k, v);
	}

	void inline put(string k, float v) {
		return SymCall("?putFloat@CompoundTag@@QEAAAEAMV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@M@Z"
			, void, void*, string, float)(this, k, v);
	}

	void inline put(string k, Tag** v) {
		return SymCall("?putCompound@CompoundTag@@QEAAPEAV1@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$unique_ptr@VCompoundTag@@U?$default_delete@VCompoundTag@@@std@@@3@@Z",
			void, void*, string, Tag**)(this, k, v);
	}

	void inline put(string k, char v) {
		return SymCall("?putByte@CompoundTag@@QEAAAEAEV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@E@Z"
			, void, void*, string, char)(this, k, v);
	}

	void inline addValue2List(Tag* t) {
		SymCall("?add@ListTag@@QEAAXV?$unique_ptr@VTag@@U?$default_delete@VTag@@@std@@@std@@@Z",
			void, void*, Tag**)(this, &t);
	}

    static inline Tag* fromItem(ItemStack* item) {
		Tag* tmp = 0;
		SymCall("?save@ItemStackBase@@QEBA?AV?$unique_ptr@VCompoundTag@@U?$default_delete@VCompoundTag@@@std@@@std@@XZ",
			void*, void*, Tag**)(item, &tmp);
		return tmp;
	}

    inline void setItem(ItemStack* item) {
		SymCall("?fromTag@ItemStack@@SA?AV1@AEBVCompoundTag@@@Z",
			void, void*, void*)(item, this);
	}
};