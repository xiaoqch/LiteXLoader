#pragma once
#include "ScriptX.h"
#include "APIHelp.h"
using namespace script;

//////////////////// Classes ////////////////////

class FormClass : public ScriptClass
{
private:
    fifo_json formJson;

public:
	FormClass();

    Local<Value> addLabel(const Arguments& args);
    Local<Value> addInput(const Arguments& args);
    Local<Value> addSwitch(const Arguments& args);
    Local<Value> addDropdown(const Arguments& args);
    Local<Value> addSlider(const Arguments& args);
    Local<Value> addStepSlider(const Arguments& args);

    Local<Value> send(const Arguments& args);
};

Local<Value> SendSimpleForm(const Arguments& args);
Local<Value> SendModelForm(const Arguments& args);
Local<Value> GiveUpForm(const Arguments& args);

Local<Value> CreateForm(const Arguments& args);