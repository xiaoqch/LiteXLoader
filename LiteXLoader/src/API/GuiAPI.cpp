#include "APIhelp.h"
#include "GuiAPI.h"
#include <Kernel/Gui.h>
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<FormClass> FormClassBuilder =
    defineClass<FormClass>("Form")
        .constructor(nullptr)
        .instanceFunction("setTitle", &FormClass::setTitle)
        .instanceFunction("addLabel", &FormClass::addLabel)
        .instanceFunction("addInput", &FormClass::addInput)
        .instanceFunction("addSwitch", &FormClass::addSwitch)
        .instanceFunction("addDropdown", &FormClass::addDropdown)
        .instanceFunction("addSlider", &FormClass::addSlider)
        .instanceFunction("addStepSlider", &FormClass::addStepSlider)
        .build();


//////////////////// Classes ////////////////////

FormClass::FormClass()
    :ScriptClass(ScriptClass::ConstructFromCpp<FormClass>{})
{ 
    form = JSON_VALUE::parse(R"({ "title":"","type":"custom_form", "content":[] })");
}


//生成函数
Local<Object> FormClass::newForm()
{
    auto newp = new FormClass();
    return newp->getScriptObject();
}

JSON_ROOT* FormClass::extractForm(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<FormClass>(v))
        return EngineScope::currentEngine()->getNativeInstance<FormClass>(v)->get();
    else
        return nullptr;
}


//成员函数
Local<Value> FormClass::setTitle(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        form["title"] = args[0].toStr();
        return Boolean::newBoolean(true);
    }
    catch(JSON_VALUE::exception &e){
        ERROR("Fail to Form currect Form string!");
        ERRPRINT(e.what());
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in setTitle!")
}

Local<Value> FormClass::addLabel(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        JSON_VALUE itemAdd;
        itemAdd["type"] = "label";
        itemAdd["text"] = args[0].toStr();

        form["content"].push_back(itemAdd);
        return Boolean::newBoolean(true);
    }
    catch(JSON_VALUE::exception &e){
        ERROR("Fail to Form currect Form string!");
        ERRPRINT(e.what());
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in addLabel!")
}

Local<Value> FormClass::addInput(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    if(args.size() >= 2)
        CHECK_ARG_TYPE(args[1],ValueKind::kString);
    if(args.size() >= 3)
        CHECK_ARG_TYPE(args[2],ValueKind::kString);

    try{
        JSON_VALUE itemAdd;
        itemAdd["type"] = "input";
        itemAdd["text"] = args[0].toStr();
        itemAdd["placeholder"] = args.size() >= 2 ? args[1].toStr() : "";
        itemAdd["default"] = args.size() >= 3 ? args[2].toStr() : "";
        
        form["content"].push_back(itemAdd);
        return Boolean::newBoolean(true);
    }
    catch(JSON_VALUE::exception &e){
        ERROR("Fail to Form currect Form string!");
        ERRPRINT(e.what());
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in addInput!")
}

Local<Value> FormClass::addSwitch(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    if(args.size() >= 2)
        CHECK_ARG_TYPE(args[1],ValueKind::kBoolean);

    try{
        JSON_VALUE itemAdd;
        itemAdd["type"] = "toggle";
        itemAdd["text"] = args[0].toStr();
        if(args.size() >= 2)
            itemAdd["default"] = args[1].asBoolean().value();
        
        form["content"].push_back(itemAdd);
        return Boolean::newBoolean(true);
    }
    catch(JSON_VALUE::exception &e){
        ERROR("Fail to Form currect Form string!");
        ERRPRINT(e.what());
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in addSwitch!")
}

Local<Value> FormClass::addDropdown(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kArray);
    if(args.size() >= 3)
        CHECK_ARG_TYPE(args[2],ValueKind::kNumber);

    try{
        JSON_VALUE itemAdd;
        itemAdd["type"] = "dropdown";
        itemAdd["text"] = args[0].toStr();
        itemAdd["options"] = ValueToJson(args[1].asArray());
        itemAdd["default"] = args.size() >= 3 ? args[2].asNumber().toInt32() : 0;
        
        form["content"].push_back(itemAdd);
        return Boolean::newBoolean(true);
    }
    catch(JSON_VALUE::exception &e){
        ERROR("Fail to Form currect Form string!");
        ERRPRINT(e.what());
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in addDropdown!")
}

Local<Value> FormClass::addSlider(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,3)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber);
    CHECK_ARG_TYPE(args[2],ValueKind::kNumber);
    if(args.size() >= 4)
        CHECK_ARG_TYPE(args[3],ValueKind::kNumber);
    if(args.size() >= 5)
        CHECK_ARG_TYPE(args[4],ValueKind::kNumber);

    try{
        JSON_VALUE itemAdd;
        itemAdd["type"] = "slider";
        itemAdd["text"] = args[0].toStr();

        int minValue = args[1].asNumber().toInt32();
        int maxValue = args[2].asNumber().toInt32();
        if(minValue >= maxValue)
            maxValue = minValue + 1;
        
        itemAdd["min"] = minValue;
        itemAdd["max"] = maxValue;
        if(args.size() >= 4)
            itemAdd["step"] = args[3].asNumber().toInt32();
        if(args.size() >= 5)
        {
            int defValue = args[4].asNumber().toInt32();
            if(defValue < minValue || defValue > maxValue)
                defValue = minValue;
            itemAdd["default"] = defValue;
        }
        
        form["content"].push_back(itemAdd);
        return Boolean::newBoolean(true);
    }
    catch(JSON_VALUE::exception &e){
        ERROR("Fail to Form currect Form string!");
        ERRPRINT(e.what());
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in addSlider!")
}

Local<Value> FormClass::addStepSlider(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kArray);
    if(args.size() >= 3)
        CHECK_ARG_TYPE(args[2],ValueKind::kNumber);

    try{
        JSON_VALUE itemAdd;
        itemAdd["type"] = "step_slider";
        itemAdd["text"] = args[0].toStr();
        itemAdd["steps"] = ValueToJson(args[1].asArray());
        int maxIndex = args[1].asArray().size()-1;
        if(args.size() >= 3)
        {
            int defIndex = args[2].asNumber().toInt32();
            if(defIndex > maxIndex)
                defIndex = maxIndex;
            itemAdd["default"] = defIndex;
        }
        
        form["content"].push_back(itemAdd);
        return Boolean::newBoolean(true);
    }
    catch(JSON_VALUE::exception &e){
        ERROR("Fail to Form currect Form string!");
        ERRPRINT(e.what());
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in addStepSlider!")
}


//////////////////// APIs ////////////////////

Local<Value> CancelForm(const Arguments& args)
{
    //############### FIX HERE ###############
    return Boolean::newBoolean(true);
}

Local<Value> NewForm(const Arguments& args)
{
    return FormClass::newForm();
}