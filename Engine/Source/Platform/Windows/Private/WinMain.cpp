// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WindowsIncludes.h"
#include "GameEngine.h"
#include "GameModule.h"
#include "PlatformMisc/PlatformModule.h"
#include "Misc/CommandLine.h"
#include "WindowsApplication.h"

DECLARE_LOG_CATEGORY(, LogWindowsLaunch);

using namespace std::chrono;

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR cmd, _In_ INT show)
{
	SCommandLine CommandArgs = StringUtils::Split(cmd, L" ", true, true);
	size_t GameModuleIdx = CommandArgs.GetArgument(L"--GameDll");
	if (GameModuleIdx == -1)
	{
		SE_LOG(LogWindowsLaunch, Fatal, L"GameModule does not specified.");
		return -1;
	}

	std::optional<std::wstring_view> ModuleName = CommandArgs.GetArgument(GameModuleIdx + 1);
	if (!ModuleName)
	{
		SE_LOG(LogWindowsLaunch, Fatal, L"GameModule does not specified.");
		return -1;
	}

	std::optional<std::wstring> EngineName;
	if (size_t EngineModuleIdx = CommandArgs.GetArgument(L"--EngineDll"); EngineModuleIdx != -1)
	{
		EngineName = CommandArgs.GetArgument(EngineModuleIdx + 1);
	}

	if (!EngineName.has_value())
	{
#if defined(_DEBUG) && 0
		constexpr const wchar_t* GameEngineModuleName = L"EditorEngine.dll";
#else
		constexpr const wchar_t* GameEngineModuleName = L"Game.dll";
#endif
		EngineName = GameEngineModuleName;
	}

	SPlatformModule EngineModule(*EngineName);
	auto loader = EngineModule.GetFunctionPointer<SGameModule*(SObject*)>("LoadGameModule");
	if (!loader)
	{
		SE_LOG(LogWindowsLaunch, Fatal, L"GameEngine does not initialized. {} is corrupted.", *EngineName);
		return -1;
	}

	SGameModule* gameModule = loader(&EngineModule);
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

	SWindowsApplication WinApp(hInstance);
	int32 errorCode = gameEngine->InvokedMain(&WinApp, *ModuleName);
	if (errorCode != 0)
	{
		SE_LOG(LogWindowsLaunch, Error, L"Application has one more error({}).", errorCode);
	}

	// Cleanup GameEngineModule.
	EngineModule.DestroyObject(gameModule);
	return errorCode;
}