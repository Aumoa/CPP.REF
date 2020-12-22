// Copyright 2020 Aumoa.lib. All right reserved.

#include "GameInstance.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;
using namespace std;

TRefPtr<String> GameInstance::defaultAppName = L"GameInstance";

GameInstance::GameInstance()
{

}

GameInstance::~GameInstance()
{

}

TRefPtr<String> GameInstance::ToString() const
{
	return defaultAppName;
}