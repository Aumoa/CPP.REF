// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameApp.h"
#include "Reflection/ConstructorInfo.h"
#include "Reflection/PropertyInfo.h"
#include "GameApp.gen.cpp"

GameApp::GameApp()
	: Super()
{
	_objValue = gcnew Object();
	_intValue = 10;
}

int32 GameApp::Run()
{
	auto props = GetType()->GetProperties();

	Object* ptr = gcnew Object();
	GC::Collect();
	Object* objValue = props[0]->GetValue<Object>(this);
	int32 intValue = props[1]->GetValue<int32>(this);
	return 0;
}