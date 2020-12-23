// Copyright 2020 Aumoa.lib. All right reserved.

#include "THAPI.h"
#include "Application.h"

#include "Logging/LogCategoryBase.h"
#include "Logging/LogVerbosity.h"

import THGameInstance;

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;
using namespace SC::Runtime::Game::Logging;

class Program : virtual public Object
{
	static LogCategoryBase LogTH;

public:
	static int32 Main()
	{
		auto app = NewObject<Application>();
		return app->Run<THGameInstance>();
	}
};

LogCategoryBase Program::LogTH(ELogVerbosity::Verbose, L"LogTH");

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE _, _In_ LPSTR, _In_ INT)
{
	return Program::Main();
}