#include "pch.h"
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

	static Tag* createTag(TagType t) {
		Tag* tag = 0;
		SymCall("?newTag@Tag@@SA?AV?$unique_ptr@VTag@@U?$default_delete@VTag@@@std@@@std@@W4Type@1@@Z",
			void, Tag**, TagType)(&tag, t);
		return tag;
	}

	char inline getTagType() {
		return (*(__int64(__fastcall**)(const struct Tag*))(*(uintptr_t*)this + 40i64))(this);
	}
};