// Copyright 2020 Aumoa.lib. All right reserved.

#include "Application.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE _, _In_ LPSTR, _In_ INT)
{
	auto app = Object::NewObject<Application>(L"GameApp");
	return app->Run();
}