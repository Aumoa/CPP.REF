// Copyright 2020 Aumoa.lib. All right reserved.

#include "Application.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE _, char*, INT)
{
	auto app = Object::NewObject<Application>("MyApp");

	return 0;
}