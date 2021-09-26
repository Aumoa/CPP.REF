// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <crtdbg.h>
#include <Windows.h>
#include "CoreWindow.h"
#include "GameEngine.h"
#include "GameModule.h"
#include "PlatformMisc/PlatformModule.h"
#include "Misc/CommandLine.h"

DECLARE_LOG_CATEGORY(, LogWindowsLaunch);

using namespace std::chrono;

INT __stdcall wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR cmd, _In_ INT show)
{
	SCommandLine commandArgs = StringUtils::Split(cmd, L" ", true, true);
	size_t gameModuleIdx = commandArgs.GetArgument(L"--GameDll");
	if (gameModuleIdx == -1)
	{
		SE_LOG(LogWindowsLaunch, Fatal, L"GameModule does not specified.");
		return -1;
	}

	std::optional<std::wstring_view> moduleName = commandArgs.GetArgument(gameModuleIdx + 1);
	if (!moduleName)
	{
		SE_LOG(LogWindowsLaunch, Fatal, L"GameModule does not specified.");
		return -1;
	}

	std::optional<std::wstring> engineName;
	if (size_t engineModuleIdx = commandArgs.GetArgument(L"--EngineDll"); engineModuleIdx != -1)
	{
		engineName = commandArgs.GetArgument(engineModuleIdx + 1);
	}

	if (!engineName.has_value())
	{
#if defined(_DEBUG)
		constexpr const wchar_t* GameEngineModuleName = L"EditorEngine.dll";
#else
		constexpr const wchar_t* GameEngineModuleName = L"Game.dll";
#endif
		engineName = GameEngineModuleName;
	}

	SPlatformModule engineModule(*engineName);
	auto loader = engineModule.GetFunctionPointer<SGameModule*(SObject*)>("LoadGameModule");
	if (!loader)
	{
		SE_LOG(LogWindowsLaunch, Fatal, L"GameEngine does not initialized. {} is corrupted.", *engineName);
		return -1;
	}

	SGameModule* gameModule = loader(&engineModule);
	if (gameModule == nullptr)
	{
		SE_LOG(LogWindowsLaunch, Fatal, L"LoadGameModule function does not defined. Please DEFINE_GAME_MODULE to any code file in module project to provide loader.");
		return -1;
	}

	SGameEngine* gameEngine = gameModule->CreateGameEngine();
	if (gameEngine == nullptr)
	{
		SE_LOG(LogWindowsLaunch, Fatal, L"Could not create GameEngine. CreateGameEngine function on GameModule return nullptr.");
		return -1;
	}

	SCoreWindow frameworkView;
	int32 errorCode = gameEngine->InvokedMain(&frameworkView, *moduleName);
	if (errorCode != 0)
	{
		SE_LOG(LogWindowsLaunch, Error, L"Application has one more error({}).", errorCode);
	}

	// Cleanup GameEngineModule.
	SObject::DestroySubobject(gameModule);
	return errorCode;
}