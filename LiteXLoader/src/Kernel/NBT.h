#include "Global.h"
#include <string>

// From BDSPyRunner
// https://github.com/twoone-3/BDSpyrunner
/*
struct Tag {
	void* _vtable;
	char _this[24];

	void put(const string& key, const Tag* value) {
		return SYMCALL("?put@CompoundTag@@QEAAAEAVTag@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$$QEAV2@@Z",
			this, key, value);
	}
	void putByte(const string& key, unsigned char value) {
		return SYMCALL("?putByte@CompoundTag@@QEAAAEAEV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@E@Z",
			this, key, value);
	}
	void putShort(const string& key, short value) {
		return SYMCALL("?putShort@CompoundTag@@QEAAAEAFV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@F@Z",
			this, key, value);
	}
	void putString(const string& key, const string& value) {
		return SYMCALL("?putString@CompoundTag@@QEAAAEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V23@0@Z",
			this, key, value);
	}
	void putInt(const string& key, int value) {
		return SYMCALL("?putInt@CompoundTag@@QEAAAEAHV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@Z",
			this, key, value);
	}
	void putInt64(const string& key, long long value) {
		return SYMCALL("?putInt64@CompoundTag@@QEAAAEA_JV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_J@Z",
			this, key, value);
	}
	void putFloat(const string& key, float value) {
		return SYMCALL("?putFloat@CompoundTag@@QEAAAEAMV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@M@Z",
			this, key, value);
	}
	void putByteArray(const string& key, const TagMemoryChunk& value) {
		return SYMCALL("?putByteArray@CompoundTag@@QEAAAEAUTagMemoryChunk@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@U2@@Z",
			this, key, &value);
	}
	void putCompound(const string& key, const Tag* value) {
		return SYMCALL("?putCompound@CompoundTag@@QEAAPEAV1@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$unique_ptr@VCompoundTag@@U?$default_delete@VCompoundTag@@@std@@@3@@Z",
			this, key, &value);
	}

	void add(Tag* t) {
		SYMCALL("?add@ListTag@@QEAAXV?$unique_ptr@VTag@@U?$default_delete@VTag@@@std@@@std@@@Z",
			this, &t);
	}
	void deCompound() {
		SYMCALL("??1CompoundTag@@UEAA@XZ", this);
	}
	void deList() {
		SYMCALL("??1ListTag@@UEAA@XZ", this);
	}

	TagType getVariantType() {
		return *((TagType*)this + 40);
	}
	TagType getListType() {
		return *((TagType*)this + 32);
	}
	auto& asByte() { return *(unsigned char*)((VA)this + 8); }
	auto& asShort() { return *(short*)((VA)this + 8); }
	auto& asInt() { return *(int*)((VA)this + 8); }
	auto& asInt64() { return *(long long*)((VA)this + 8); }
	auto& asFloat() { return *(float*)((VA)this + 8); }
	auto& asDouble() { return *(double*)((VA)this + 8); }
	auto& asString() { return *(string*)((VA)this + 8); }
	auto& asByteArray() { return *(TagMemoryChunk*)((VA)this + 8); }
	auto asListTag() { return (Tag*)this; }
	auto& asList() { return *(vector<Tag*>*)((VA)this + 8); }
	auto& asCompound() { return *(map<string, Tag>*)((VA)this + 8); }
};*/