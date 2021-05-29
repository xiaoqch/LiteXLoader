#pragma once
#include "ScriptX.h"
#include "APIHelp.h"
using namespace script;

//////////////////// Classes ////////////////////

class FormClass : public ScriptClass
{
private:
    JSON_ROOT form;

public:
	FormClass();

    JSON_ROOT *get()
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

Local<Value> CancelForm(const Arguments& args);

Local<Value> NewForm(const Arguments& args);