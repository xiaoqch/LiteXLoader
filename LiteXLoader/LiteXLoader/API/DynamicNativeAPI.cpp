#include "APIHelp.h"
#include "DynamicNativeAPI.h"
#include <Kernel/DynamicNative.h>
#include <unordered_map>
#include <string>
#include "PlayerAPI.h"
#include "BlockAPI.h"
#include "EntityAPI.h"
#include "ItemAPI.h"
#include "BlockEntityAPI.h"
#include "ContainerAPI.h"
#include "ScoreboardAPI.h"
using namespace std;

//////////////////// Symbols ////////////////////

struct SymbolTypeData
{
	NativeTypes ret;
	vector<NativeTypes> paras;
	void* func;
};

DCCallVM* vm;
unordered_map<string, SymbolTypeData> symbols;
vector<pair<NativeTypes, void*>> garbage;

void InitDynamicCallSystem()
{
	vm = dcNewCallVM(4096);
	dcMode(vm, DC_CALL_C_DEFAULT);
}

void GarbageClean()
{
	for (auto& [t, v] : garbage)
	{
		switch (t)
		{
		case NativeTypes::String:
			delete (string*)v;
		default:
			break;
		}
	}
}


//////////////////// Hook ////////////////////

struct DyHookData
{
	string sym;
	void* origin;
	DCCallback* cb;

	ScriptEngine* engine;
	Global<Function> callback;

	DyHookData(const string& sym, ScriptEngine* cur, Local<Function> callback)
	{
		this->sym = sym;
		this->engine = cur;
		this->callback = callback;
	}
};

list<DyHookData> hookData;

char CallbackHandler(DCCallback* cb, DCArgs* args, DCValue* result, void* userdata)
{
	DyHookData* hookInfo = (DyHookData*)userdata;
	SymbolTypeData sym = symbols[hookInfo->sym];

	EngineScope enter(hookInfo->engine);
	try
	{
		vector<Local<Value>> paras;
		for (int i = 0; i < sym.paras.size(); ++i)
		{
			switch (sym.paras[i])
			{
			case NativeTypes::Bool:
				paras.emplace_back(Boolean::newBoolean(dcbArgBool(args)));
				break;
			case NativeTypes::Char:
			case NativeTypes::Int8:
				paras.emplace_back(Number::newNumber(dcbArgChar(args)));
				break;
			case NativeTypes::UnsignedChar:
				paras.emplace_back(Number::newNumber(dcbArgUChar(args)));
				break;
			case NativeTypes::Short:
			case NativeTypes::Int16:
				paras.emplace_back(Number::newNumber(dcbArgShort(args)));
				break;
			case NativeTypes::UnsignedShort:
				paras.emplace_back(Number::newNumber(dcbArgUShort(args)));
				break;
			case NativeTypes::Int:
			case NativeTypes::Int32:
				paras.emplace_back(Number::newNumber(dcbArgInt(args)));
				break;
			case NativeTypes::UnsignedInt:
				paras.emplace_back(Number::newNumber((int)dcbArgUInt(args)));
				break;
			case NativeTypes::Long:
				paras.emplace_back(Number::newNumber((int)dcbArgLong(args)));
				break;
			case NativeTypes::UnsignedLong:
				paras.emplace_back(Number::newNumber((int)dcbArgULong(args)));
				break;
			case NativeTypes::LongLong:
			case NativeTypes::Int64:
				paras.emplace_back(Number::newNumber(dcbArgLongLong(args)));
				break;
			case NativeTypes::UnsignedLongLong:
				paras.emplace_back(Number::newNumber((long long)dcbArgULongLong(args)));
				break;
			case NativeTypes::Float:
				paras.emplace_back(Number::newNumber(dcbArgFloat(args)));
				break;
			case NativeTypes::Double:
				paras.emplace_back(Number::newNumber(dcbArgDouble(args)));
				break;
			case NativeTypes::Pointer:
				paras.emplace_back(Number::newNumber((intptr_t)dcbArgPointer(args)));
				break;
			case NativeTypes::String:
				paras.emplace_back(String::newString(*(string*)dcbArgPointer(args)));
				break;
			//Custom Class
			case NativeTypes::Player:
				paras.emplace_back(PlayerClass::newPlayer((Player*)dcbArgPointer(args)));
				break;
			case NativeTypes::Block:
				//###################### Fix Pos Info ######################
				paras.emplace_back(BlockClass::newBlock((Block*)dcbArgPointer(args)));
				break;
			case NativeTypes::Entity:
				paras.emplace_back(EntityClass::newEntity((Actor*)dcbArgPointer(args)));
				break;
			case NativeTypes::ItemStack:
				paras.emplace_back(ItemClass::newItem((ItemStack*)dcbArgPointer(args)));
				break;
			case NativeTypes::BlockEntity:
				//###################### Fix Pos Info ######################
				paras.emplace_back(BlockEntityClass::newBlockEntity((BlockActor*)dcbArgPointer(args), 0));
				break;
			case NativeTypes::Container:
				paras.emplace_back(ContainerClass::newContainer((Container*)dcbArgPointer(args)));
				break;
			default:
				break;
			}
		}

		Local<Value> res = hookInfo->callback.get().call({}, paras);

		switch (sym.ret)
		{
		case NativeTypes::Bool:
			result->B = res.asBoolean().value();
			break;
		case NativeTypes::Char:
		case NativeTypes::Int8:
			result->c = (char)res.toInt();
			break;
		case NativeTypes::UnsignedChar:
			result->C = (unsigned char)res.toInt();
			break;
		case NativeTypes::Short:
		case NativeTypes::Int16:
			result->s = (short)res.toInt();
			break;
		case NativeTypes::UnsignedShort:
			result->S = (unsigned short)res.toInt();
			break;
		case NativeTypes::Int:
		case NativeTypes::Int32:
			result->i = (int)res.toInt();
			break;
		case NativeTypes::UnsignedInt:
			result->I = (unsigned int)res.toInt();
			break;
		case NativeTypes::Long:
			result->j = (long)res.toInt();
			break;
		case NativeTypes::UnsignedLong:
			result->J = (unsigned long)res.toInt();
			break;
		case NativeTypes::LongLong:
		case NativeTypes::Int64:
			result->l = (long long)res.asNumber().toInt64();
			break;
		case NativeTypes::UnsignedLongLong:
			result->L = (unsigned long long)res.asNumber().toInt64();
			break;
		case NativeTypes::Float:
			result->f = res.asNumber().toFloat();
			break;
		case NativeTypes::Double:
			result->d = res.asNumber().toDouble();
			break;
		case NativeTypes::Pointer:
		case NativeTypes::String:
			result->p = (void*)(uintptr_t)(res.asNumber().toInt64());
			break;
		//Custom Class
		case NativeTypes::Player:
			result->p = (void*)(uintptr_t)(PlayerClass::extractPlayer(res));
			break;
		case NativeTypes::Block:
			result->p = (void*)(uintptr_t)(BlockClass::extractBlock(res));
			break;
		case NativeTypes::Entity:
			result->p = (void*)(uintptr_t)(EntityClass::extractEntity(res));
			break;
		case NativeTypes::ItemStack:
			result->p = (void*)(uintptr_t)(ItemClass::extractItem(res));
			break;
		case NativeTypes::BlockEntity:
			result->p = (void*)(uintptr_t)(BlockEntityClass::extractBlockEntity(res));
			break;
		case NativeTypes::Container:
			result->p = (void*)(uintptr_t)(ContainerClass::extractContainer(res));
			break;
		default:
			break;
		}
	}
	catch (const Exception& e)
	{
		ERROR("Hook Callback Failed!");
		ERRPRINT(e);
		ERROR("In Symbol: " + hookInfo->sym);
		ERROR("In Plugin: " + ENGINE_OWN_DATA()->pluginName);
	}

	return GetTypeSignature(sym.ret);
}


//////////////////// Helper ////////////////////

#define NATIVE_CHECK_ARG_TYPE(TYPE) \
	if (args[i].getKind() != TYPE) \
	{ \
		ERROR("Wrong type of argument!"); \
		ERROR("In Symbol: " + sym); \
		ERROR("In Plugin: " + ENGINE_OWN_DATA()->pluginName); \
		return Local<Value>(); \
	}

#define NATIVE_CHECK_ARG_CLASS(TYPE) \
	if (!IsInstanceOf<TYPE>(args[i])) \
	{ \
		ERROR("Wrong type of argument!"); \
		ERROR("In Symbol: " + sym); \
		ERROR("In Plugin: " + ENGINE_OWN_DATA()->pluginName); \
		return Local<Value>(); \
	}

Local<Value> CallNativeFunction(DCCallVM* vm, const string &sym, void *callfunc, const Arguments& args)
{
	SymbolTypeData& data = symbols[sym];

	if (args.size() < data.paras.size())
	{
		ERROR("Too Few arguments!");
		ERROR("In Symbol: " + sym);
		ERROR("In Plugin: " + ENGINE_OWN_DATA()->pluginName);
		return Local<Value>();
	}

	dcReset(vm);
	for (int i = 0; i < data.paras.size(); ++i)
	{
		switch (data.paras[i])
		{
		case NativeTypes::Bool:
			NATIVE_CHECK_ARG_TYPE(ValueKind::kBoolean);
			dcArgBool(vm, args[i].asBoolean().value());
			break;
		case NativeTypes::Char:
		case NativeTypes::UnsignedChar:
		case NativeTypes::Int8:
			NATIVE_CHECK_ARG_TYPE(ValueKind::kNumber);
			dcArgChar(vm, (char)args[i].asNumber().toInt32());
			break;
		case NativeTypes::Short:
		case NativeTypes::UnsignedShort:
		case NativeTypes::Int16:
			NATIVE_CHECK_ARG_TYPE(ValueKind::kNumber);
			dcArgShort(vm, (short)args[i].asNumber().toInt32());
			break;
		case NativeTypes::Int:
		case NativeTypes::UnsignedInt:
		case NativeTypes::Int32:
			NATIVE_CHECK_ARG_TYPE(ValueKind::kNumber);
			dcArgInt(vm, (int)args[i].asNumber().toInt32());
			break;
		case NativeTypes::Long:
		case NativeTypes::UnsignedLong:
			NATIVE_CHECK_ARG_TYPE(ValueKind::kNumber);
			dcArgLong(vm, (long)args[i].asNumber().toInt32());
			break;
		case NativeTypes::LongLong:
		case NativeTypes::UnsignedLongLong:
		case NativeTypes::Int64:
			NATIVE_CHECK_ARG_TYPE(ValueKind::kNumber);
			dcArgLongLong(vm, args[i].asNumber().toInt64());
			break;
		case NativeTypes::Float:
			NATIVE_CHECK_ARG_TYPE(ValueKind::kNumber);
			dcArgFloat(vm, args[i].asNumber().toFloat());
			break;
		case NativeTypes::Double:
			NATIVE_CHECK_ARG_TYPE(ValueKind::kNumber);
			dcArgDouble(vm, args[i].asNumber().toDouble());
			break;
		case NativeTypes::Pointer:
			NATIVE_CHECK_ARG_TYPE(ValueKind::kNumber);
			dcArgPointer(vm, (void*)(uintptr_t)args[i].asNumber().toInt64());
			break;
		case NativeTypes::String:
		{
			NATIVE_CHECK_ARG_TYPE(ValueKind::kString);
			string* str = new string(args[i].toStr());
			garbage.push_back({ NativeTypes::String, (void*)str });
			dcArgPointer(vm, (void*)(uintptr_t)str);
			break;
		}
		// Custom Classes
		case NativeTypes::Player:
			NATIVE_CHECK_ARG_CLASS(PlayerClass);
			dcArgPointer(vm, PlayerClass::extractPlayer(args[i]));
			break;
		case NativeTypes::Block:
			NATIVE_CHECK_ARG_CLASS(BlockClass);
			dcArgPointer(vm, BlockClass::extractBlock(args[i]));
			break;
		case NativeTypes::Entity:
			NATIVE_CHECK_ARG_CLASS(EntityClass);
			dcArgPointer(vm, EntityClass::extractEntity(args[i]));
			break;
		case NativeTypes::ItemStack:
			NATIVE_CHECK_ARG_CLASS(ItemClass);
			dcArgPointer(vm, ItemClass::extractItem(args[i]));
			break;
		case NativeTypes::BlockEntity:
			NATIVE_CHECK_ARG_CLASS(BlockEntityClass);
			dcArgPointer(vm, BlockEntityClass::extractBlockEntity(args[i]));
			break;
		case NativeTypes::Container:
			NATIVE_CHECK_ARG_CLASS(ContainerClass);
			dcArgPointer(vm, ContainerClass::extractContainer(args[i]));
			break;
		default:
			break;
		}
	}

	Local<Value> res;
	DCpointer func = (DCpointer)callfunc;
	switch (data.ret)
	{
	case NativeTypes::Void:
		dcCallVoid(vm, func);
		break;
	case NativeTypes::Bool:
		res = Boolean::newBoolean(dcCallBool(vm, func));
		break;
	case NativeTypes::Char:
	case NativeTypes::UnsignedChar:
	case NativeTypes::Int8:
		res = Number::newNumber(dcCallChar(vm, func));
		break;
	case NativeTypes::Short:
	case NativeTypes::UnsignedShort:
	case NativeTypes::Int16:
		res = Number::newNumber(dcCallShort(vm, func));
		break;
	case NativeTypes::Int:
	case NativeTypes::UnsignedInt:
	case NativeTypes::Int32:
		res = Number::newNumber(dcCallInt(vm, func));
		break;
	case NativeTypes::Long:
	case NativeTypes::UnsignedLong:
		res = Number::newNumber((int)dcCallLong(vm, func));
		break;
	case NativeTypes::LongLong:
	case NativeTypes::UnsignedLongLong:
	case NativeTypes::Int64:
		res = Number::newNumber(dcCallLongLong(vm, func));
		break;
	case NativeTypes::Float:
		res = Number::newNumber(dcCallFloat(vm, func));
		break;
	case NativeTypes::Double:
		res = Number::newNumber(dcCallDouble(vm, func));
		break;
	case NativeTypes::Pointer:
		res = Number::newNumber((long long)(uintptr_t)dcCallPointer(vm, func));
		break;
	case NativeTypes::String:
	{
		res = String::newString(*(string*)dcCallPointer(vm, func));
		break;
	}
	// Custom Classes
	case NativeTypes::Player:
		res = PlayerClass::newPlayer((Player*)dcCallPointer(vm, func));
		break;
	case NativeTypes::Block:
		//###################### Fix Pos Info ######################
		res = BlockClass::newBlock((Block*)dcCallPointer(vm, func));
		break;
	case NativeTypes::Entity:
		res = EntityClass::newEntity((Actor*)dcCallPointer(vm, func));
		break;
	case NativeTypes::ItemStack:
		res = ItemClass::newItem((ItemStack*)dcCallPointer(vm, func));
		break;
	case NativeTypes::BlockEntity:
		//###################### Fix Pos Info ######################
		res = BlockEntityClass::newBlockEntity((BlockActor*)dcCallPointer(vm, func), 0);
		break;
	case NativeTypes::Container:
		res = ContainerClass::newContainer((Container*)dcCallPointer(vm, func));
		break;
	default:
		break;
	}

	GarbageClean();
	return res;
}


//////////////////// Class ////////////////////

#define	DEFINE_NATIVE_TYPE(NAME) property(#NAME, [] { return Number::newNumber((int)NativeTypes::NAME); })

ClassDefine<void> NativeClassBuilder =
	defineClass("Native")
		.DEFINE_NATIVE_TYPE(Void) .DEFINE_NATIVE_TYPE(Bool) .DEFINE_NATIVE_TYPE(Char) .DEFINE_NATIVE_TYPE(UnsignedChar)
		.DEFINE_NATIVE_TYPE(Short) .DEFINE_NATIVE_TYPE(UnsignedShort) .DEFINE_NATIVE_TYPE(Int) .DEFINE_NATIVE_TYPE(UnsignedInt)
		.DEFINE_NATIVE_TYPE(Long) .DEFINE_NATIVE_TYPE(UnsignedLong) .DEFINE_NATIVE_TYPE(LongLong) .DEFINE_NATIVE_TYPE(UnsignedLongLong)
		.DEFINE_NATIVE_TYPE(Float) .DEFINE_NATIVE_TYPE(Double) .DEFINE_NATIVE_TYPE(Pointer)

		.DEFINE_NATIVE_TYPE(Int8) .DEFINE_NATIVE_TYPE(Int16) .DEFINE_NATIVE_TYPE(Int32) .DEFINE_NATIVE_TYPE(Int64)

		.DEFINE_NATIVE_TYPE(Player) .DEFINE_NATIVE_TYPE(Block) .DEFINE_NATIVE_TYPE(Entity) .DEFINE_NATIVE_TYPE(ItemStack)
		.DEFINE_NATIVE_TYPE(BlockEntity) .DEFINE_NATIVE_TYPE(Container) .DEFINE_NATIVE_TYPE(Objective)
		.DEFINE_NATIVE_TYPE(BlockPos) .DEFINE_NATIVE_TYPE(Vec3)
		
		.DEFINE_NATIVE_TYPE(String)

		.function("getSymbol", &NativeClass::getSymbol)
		.function("hook", &NativeClass::hook)
		.build();


//////////////////// APIs ////////////////////

Local<Value> Hook(const Arguments& args)
{
	CHECK_ARGS_COUNT(args, 3);
	CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
	CHECK_ARG_TYPE(args[1], ValueKind::kString);
	CHECK_ARG_TYPE(args[args.size()-1], ValueKind::kFunction);

	try
	{
		//GetSymbol
		string sym{ args[1].toStr() };

		if (symbols.find(sym) == symbols.end())
		{
			SymbolTypeData& data = symbols[sym];
			data.ret = (NativeTypes)args[0].toInt();
			for (int i = 2; i < args.size() - 1; ++i)
				data.paras.push_back((NativeTypes)args[i].toInt());

			data.func = ResolveSymbol(sym.c_str());
			if (!data.func)
			{
				ERROR("Fail to hook: " + sym);
				ERROR(std::string("In API: ") + __FUNCTION__);
				ERROR("In Plugin: " + ENGINE_OWN_DATA()->pluginName);
				return Local<Value>();
			}
		}

		//Init Origin
		hookData.emplace_back(sym,EngineScope::currentEngine(), args[args.size() - 1].asFunction());
		DyHookData* hookInfo = &hookData.back();

		//Init Callback
		string callbackStr;

		SymbolTypeData& data = symbols[sym];
		for (int i = 0; i < data.paras.size(); ++i)
		{
			callbackStr.append(GetTypeSignature(data.paras[i]), 1);
		}
		callbackStr.append(')', 1);
		callbackStr.append(GetTypeSignature(data.ret), 1);

		hookInfo->cb = dcbNewCallback(callbackStr.c_str(), &CallbackHandler, hookInfo);
		
		//Hook
		HookSymbol(sym, &hookInfo->origin, hookInfo->cb);
		
		return Function::newFunction([sym, hookInfo](const Arguments& args)->Local<Value>
		{
			SymbolTypeData& data = symbols[sym];

			if (args.size() < data.paras.size())
			{
				ERROR("Too Few arguments in Hook Callback!");
				ERROR("In Symbol: " + sym);
				ERROR("In Plugin: " + ENGINE_OWN_DATA()->pluginName);
				return Local<Value>();
			}

			return CallNativeFunction(vm, sym, hookInfo->origin, args);
		});

	}
	CATCH("Fail in Hook!")
}

Local<Value> GetSymbol(const Arguments& args)
{
	CHECK_ARGS_COUNT(args, 2);
	CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
	CHECK_ARG_TYPE(args[1], ValueKind::kString);

	try
	{
		string sym{ args[1].toStr() };

		if (symbols.find(sym) == symbols.end())
		{
			SymbolTypeData& data = symbols[sym];
			data.ret = (NativeTypes)args[0].toInt();
			for (int i = 2; i < args.size(); ++i)
				data.paras.push_back((NativeTypes)args[i].toInt());

			data.func = ResolveSymbol(sym.c_str());
			if (!data.func)
			{
				ERROR("Fail to hook: " + sym);
				ERROR(std::string("In API: ") + __FUNCTION__);
				ERROR("In Plugin: " + ENGINE_OWN_DATA()->pluginName);
				return Local<Value>();
			}
		}

		return Function::newFunction([sym](const Arguments& args) -> Local<Value>
		{
			SymbolTypeData& data = symbols[sym];

			if (args.size() < data.paras.size())
			{
				ERROR("Too Few arguments!");
				ERROR("In Symbol: " + sym);
				ERROR("In Plugin: " + ENGINE_OWN_DATA()->pluginName);
				return Local<Value>();
			}

			return CallNativeFunction(vm, sym, data.func, args);
		});
	}
	CATCH("Fail in GetSymbol!")
}