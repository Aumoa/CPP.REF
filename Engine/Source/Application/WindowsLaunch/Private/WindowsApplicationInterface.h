// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlatformCore/IApplicationInterface.h"
#include "WindowsIncludes.h"

class SWindowsApplicationInterface : implements SObject, implements IApplicationInterface
{
	GENERATED_BODY(SWindowsApplicationInterface)

private:
	HINSTANCE hInstance;
	std::wstring CmdLine;

	HWND hWnd = nullptr;

public:
	static SWindowsApplicationInterface* GApp;

public:
	SWindowsApplicationInterface(HINSTANCE hInstance, std::wstring_view CmdLine);

	virtual std::wstring GetCommandLine() override;
	virtual void Start() override;
	virtual Vector2N GetViewportSize() override;

	virtual IRHIDevice* CreateDevice() override;

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};