#include "APIHelp.h"
#include "GuiAPI.h"
#include <Kernel/Gui.h>
#include <iostream>
using namespace script;
using namespace std;

//////////////////// Class Definition ////////////////////

ClassDefine<SimpleFormClass> SimpleFormClassBuilder =
    defineClass<SimpleFormClass>("LXL_SimpleForm")
        .constructor(nullptr)
        .instanceFunction("setTitle", &SimpleFormClass::setTitle)
        .instanceFunction("setContent", &SimpleFormClass::setContent)
        .instanceFunction("addButton", &SimpleFormClass::addButton)
        .build();

ClassDefine<CustomFormClass> CustomFormClassBuilder =
    defineClass<CustomFormClass>("LXL_CustomForm")
        .constructor(nullptr)
        .instanceFunction("setTitle", &CustomFormClass::setTitle)
        .instanceFunction("addLabel", &CustomFormClass::addLabel)
        .instanceFunction("addInput", &CustomFormClass::addInput)
        .instanceFunction("addSwitch", &CustomFormClass::addSwitch)
        .instanceFunction("addDropdown", &CustomFormClass::addDropdown)
        .instanceFunction("addSlider", &CustomFormClass::addSlider)
        .instanceFunction("addStepSlider", &CustomFormClass::addStepSlider)
        .build();

//////////////////// Simple Form ////////////////////

SimpleFormClass::SimpleFormClass()
    :ScriptClass(ScriptClass::ConstructFromCpp<SimpleFormClass>{})
{
    form = JSON_VALUE::parse(R"({"title":"","content":"","buttons":[],"type":"form"})");
}

//生成函数
Local<Object> SimpleFormClass::newForm()
{
    auto newp = new SimpleFormClass();
    return newp->getScriptObject();
}

JSON_ROOT* SimpleFormClass::extractForm(Local<Value> v)
{
    if (EngineScope::currentEngine()->isInstanceOf<SimpleFormClass>(v))
        return EngineScope::currentEngine()->getNativeInstance<SimpleFormClass>(v)->get();
    else
        return nullptr;
}

//成员函数
Local<Value> SimpleFormClass::setTitle(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)

    try {
        form["title"] = args[0].toStr();
        return Boolean::newBoolean(true);
    }
    catch (JSON_VALUE::exception& e) {
        ERROR("Fail to Form currect Form string!");
        ERRPRINT(e.what());
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in setTitle!")
}

Local<Value> SimpleFormClass::setContent(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)

    try {
        form["content"] = args[0].toStr();
        return Boolean::newBoolean(true);
    }
    catch (JSON_VALUE::exception& e) {
        ERROR("Fail to Form currect Form string!");
        ERRPRINT(e.what());
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in setContent!")
}

Local<Value> SimpleFormClass::addButton(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)
    if (args.size() >= 2)
        CHECK_ARG_TYPE(args[1], ValueKind::kString);

    try {
        JSON_VALUE oneButton;
        oneButton["text"] = args[0].toStr();
        if (args.size() >= 2)
        {
            string path = args[1].toStr();
            JSON_VALUE image;
            image["type"] = path.find("textures/") == 0 ? "path" : "url";
            image["data"] = path;
            oneButton["image"] = image;
        }
        form["buttons"].push_back(oneButton);
        return Boolean::newBoolean(true);
    }
    catch (JSON_VALUE::exception& e) {
        ERROR("Fail to Form currect Form string!");
        ERRPRINT(e.what());
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in addButton!")
}



//////////////////// Custom Form ////////////////////

CustomFormClass::CustomFormClass()
    :ScriptClass(ScriptClass::ConstructFromCpp<CustomFormClass>{})
{ 
    form = JSON_VALUE::parse(R"({ "title":"", "type":"custom_form", "content":[], "buttons":[] })");
}


//生成函数
Local<Object> CustomFormClass::newForm()
{
    auto newp = new CustomFormClass();
    return newp->getScriptObject();
}

JSON_ROOT* CustomFormClass::extractForm(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<CustomFormClass>(v))
        return EngineScope::currentEngine()->getNativeInstance<CustomFormClass>(v)->get();
    else
        return nullptr;
}


//成员函数
Local<Value> CustomFormClass::setTitle(const Arguments& args)
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

Local<Value> CustomFormClass::addLabel(const Arguments& args)
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

Local<Value> CustomFormClass::addInput(const Arguments& args)
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

Local<Value> CustomFormClass::addSwitch(const Arguments& args)
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

Local<Value> CustomFormClass::addDropdown(const Arguments& args)
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
        itemAdd["options"] = JSON_VALUE::parse(ValueToJson(args[1].asArray()));
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

Local<Value> CustomFormClass::addSlider(const Arguments& args)
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

Local<Value> CustomFormClass::addStepSlider(const Arguments& args)
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
        itemAdd["steps"] = JSON_VALUE::parse(ValueToJson(args[1].asArray()));
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

Local<Value> NewSimpleForm(const Arguments& args)
{
    return SimpleFormClass::newForm();
}

Local<Value> NewCustomForm(const Arguments& args)
{
    return CustomFormClass::newForm();
}