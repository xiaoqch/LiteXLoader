#pragma once
#include <ScriptX/ScriptX.h>
#include <Kernel/ThirdParty.h>
using namespace script;

//////////////////// Classes ////////////////////

class SimpleFormClass : public ScriptClass
{
private:
    JSON_ROOT form;

public:
    SimpleFormClass();

    JSON_ROOT *get()
    {
        return &form;
    }

    static Local<Object> newForm();
    static JSON_ROOT* extractForm(Local<Value> v);

    Local<Value> setTitle(const Arguments& args);
    Local<Value> setContent(const Arguments& args);

    Local<Value> addButton(const Arguments& args);
};


class CustomFormClass : public ScriptClass
{
private:
    JSON_ROOT form;

public:
    CustomFormClass();

    JSON_ROOT* get()
    {
        return &form;
    }

    static Local<Object> newForm();
    static JSON_ROOT* extractForm(Local<Value> v);

    Local<Value> setTitle(const Arguments& args);

    Local<Value> addLabel(const Arguments& args);
    Local<Value> addInput(const Arguments& args);
    Local<Value> addSwitch(const Arguments& args);
    Local<Value> addDropdown(const Arguments& args);
    Local<Value> addSlider(const Arguments& args);
    Local<Value> addStepSlider(const Arguments& args);
};

//////////////////// APIs ////////////////////

Local<Value> NewSimpleForm(const Arguments& args);
Local<Value> NewCustomForm(const Arguments& args);