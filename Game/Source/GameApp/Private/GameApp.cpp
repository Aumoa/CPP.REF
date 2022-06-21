// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameApp.h"
#include "Reflection/ConstructorInfo.h"
#include "GameApp.gen.cpp"

GameApp::GameApp()
	: Super()
{
}

int32 GameApp::Run()
{
	Type* t = GetType();
	auto ctors = t->GetConstructors();
	auto ptr1 = Cast<GameApp>((Object*)ctors[0]->Invoke());
	return ptr1->Run();
}