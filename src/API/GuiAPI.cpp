#include "APIhelp.h"
#include "GuiAPI.h"
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<FormClass> FormClassBuilder =
    defineClass<FormClass>("Form")
        .constructor(nullptr)
        .instanceFunction("addLabel", &FormClass::addLabel)
        .instanceFunction("addInput", &FormClass::addInput)
        .instanceFunction("addSwitch", &FormClass::addSwitch)
        .instanceFunction("addDropdown", &FormClass::addDropdown)
        .instanceFunction("addSlider", &FormClass::addSlider)
        .instanceFunction("addStepSlider", &FormClass::addStepSlider)
        
        .instanceFunction("send", &FormClass::send)
        .build();


//////////////////// Classes ////////////////////

FormClass::FormClass()
    :ScriptClass(ScriptClass::ConstructFromCpp<FormClass>{})
{ }

Local<Value> FormClass::addLabel(const Arguments& args)
{
    return Boolean::newBoolean(true);
}

Local<Value> FormClass::addInput(const Arguments& args)
{
    return Boolean::newBoolean(true);
}

Local<Value> FormClass::addSwitch(const Arguments& args)
{
    return Boolean::newBoolean(true);
}

Local<Value> FormClass::addDropdown(const Arguments& args)
{
    return Boolean::newBoolean(true);
}

Local<Value> FormClass::addSlider(const Arguments& args)
{
    return Boolean::newBoolean(true);
}

Local<Value> FormClass::addStepSlider(const Arguments& args)
{
    return Boolean::newBoolean(true);
}

Local<Value> FormClass::send(const Arguments& args)
{
    return Boolean::newBoolean(true);
}


//////////////////// APIs ////////////////////

Local<Value> SendSimpleForm(const Arguments& args)
{
    return Boolean::newBoolean(true);
}

Local<Value> SendModelForm(const Arguments& args)
{
    return Boolean::newBoolean(true);
}

Local<Value> GiveUpForm(const Arguments& args)
{
    return Boolean::newBoolean(true);
}

Local<Value> CreateForm(const Arguments& args)
{
    return Boolean::newBoolean(true);
}