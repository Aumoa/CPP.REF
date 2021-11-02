// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IApplicationInterface.h"
#include "WindowsIncludes.h"

class SWindowsApplication : implements SObject, implements IApplicationInterface
{
	GENERATED_BODY(SWindowsApplication)

private:
	inline static SWindowsApplication* gApp;

	HINSTANCE hInstance = nullptr;
	HWND hWnd = nullptr;
	ETickMode TickMode = ETickMode::Realtime;

public:
	SWindowsApplication(HINSTANCE hInstance);

	virtual void Start() override;
	virtual Vector2N GetViewportSize() override;

	virtual void SetTickMode(ETickMode InTickMode) override;
	virtual ETickMode GetTickMode() override;

	virtual void SetTitle(std::wstring_view InTitle) override;
	virtual std::wstring GetTitle() override;

	virtual IRHIFactory* CreateFactory() override;
	virtual IPlatformKeyboard& GetPlatformKeyboard() override;
	virtual IPlatformMouse& GetPlatformMouse() override;

	HWND GetWindowHandle();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};