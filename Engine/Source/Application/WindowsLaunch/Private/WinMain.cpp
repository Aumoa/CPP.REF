// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WindowsIncludes.h"
#include "WindowsApplicationInterface.h"
#include "AppEngine.h"

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ wchar_t* CmdLine, _In_ int)
{
	std::shared_ptr Application = std::make_shared<SWindowsApplicationInterface>(hInstance, CmdLine);
	SAppEngine* Engine = Application->NewObject<SAppEngine>();
	return Engine->GuardedMain(Application.get());
}