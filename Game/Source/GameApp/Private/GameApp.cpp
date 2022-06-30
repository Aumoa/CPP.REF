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

void GameApp::Dispose()
{
	throw;
}

int32 GameApp::Run()
{
	for (auto* p = GetType(); p; p = p->GetBaseType())
	{
		std::vector<String> interfaces;
		for (auto& i : p->GetInterfaces())
		{
			interfaces.emplace_back(String::Format(TEXT("          interface {}"), i->GetName()));
		}

		std::vector<String> values;
		values.emplace_back(p->GetName());
		if (interfaces.size())
		{
			values.emplace_back(String::Join(TEXT("\n"), interfaces));
		}
		Log::Info(LogTemp, TEXT("class {}"), String::Join(TEXT("\n"), values));
	}

	return 0;
}

bool GameApp::IsValid()
{
	return true;
}