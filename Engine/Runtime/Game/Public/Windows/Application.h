// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "HandleBase.h"

class CoreWindow;
class WinKeyboard;
class WinMouse;

class GAME_API Application : public HandleBase
{
public:
	using Super = HandleBase;

private:
	static Application* instance;

	HANDLE hModule;
	TRefPtr<CoreWindow> mainWindow;
	bool bMainLoop : 1;

	TRefPtr<WinKeyboard> winKey;
	TRefPtr<WinMouse> winMouse;

protected:
	bool bForcedTick : 1;

public:
	Application();
	~Application() override;
	
	void Run(TRefPtr<CoreWindow> main);
	virtual void Tick();

	vs_property_get_auto(CoreWindow*, MainWindow, mainWindow.Get());
	vs_property_get_auto(bool, MainLoop, bMainLoop);

	static void PostQuitMessage(int32 exitCode);
	static Application* GetInstance();

protected:
	virtual void Initialize();
	virtual void Shutdown();
};