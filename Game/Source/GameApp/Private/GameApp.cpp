// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameApp.h"
#include "Reflection/ConstructorInfo.h"
#include "Reflection/PropertyInfo.h"
#include "Reflection/MethodInfo.h"
#include "GameApp.gen.cpp"

GameApp::GameApp()
	: Super()
{
}

int32 GameApp::Run()
{
	_objValue = gcnew Object();
	_intValue = 10;

	using r = std::invoke_result_t<decltype(&GameApp::Run), GameApp*>;

	auto props = GetType()->GetProperties();

	Object* ptr = gcnew Object();
	GC::Collect();
	Object* objValue = props[0]->GetValue<Object>(this);
	int32 intValue = props[1]->GetValue<int32>(this);

	auto* gameApp = Cast<GameApp>(MemberwiseClone());
	const bool b = gameApp->_objValue == _objValue;

	for (auto& method : GetType()->GetMethods())
	{
		if (method->GetName() == TEXT("Run"))
		{
			method->Invoke(this);
		}
	}
	return 0;
}

bool GameApp::IsValid()
{
	return true;
}