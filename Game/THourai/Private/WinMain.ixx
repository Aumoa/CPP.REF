// Copyright 2020 Aumoa.lib. All right reserved.

#include "Application.h"

import THGameInstance;

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;

class Program : virtual public Object
{
public:
	static int32 Main()
	{
		auto app = NewObject<Application>();
		return app->Run<THGameInstance>();
	}
};

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE _, _In_ LPSTR, _In_ INT)
{
	return Program::Main();
}