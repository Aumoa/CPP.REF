// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IApplicationInterface.h"
#include "WindowsIncludes.h"
#include "Multimedia/IPlatformImageLoader.h"

class SDXGIFactory;

class SWindowsApplication : implements SObject, implements IApplicationInterface, implements IPlatformImageLoader
{
	GENERATED_BODY(SWindowsApplication)

private:
	inline static SWindowsApplication* gApp;

	HINSTANCE hInstance = nullptr;
	HWND hWnd = nullptr;
	ETickMode TickMode = ETickMode::Realtime;
	ETickMode ActualTickMode = ETickMode::Realtime;
	std::vector<std::weak_ptr<SObject>> RealtimeDemanders;

	SDXGIFactory* Factory = nullptr;
	ComPtr<IWICImagingFactory> ImagingFactory;

public:
	SWindowsApplication(HINSTANCE hInstance);

	// IApplicationInterface
	virtual void Start() override;
	virtual Vector2N GetViewportSize() override;
	virtual float GetDpi() override;

	virtual void SetTickMode(ETickMode InTickMode) override;
	virtual ETickMode GetTickMode() override;
	virtual void AddRealtimeDemander(SObject* InObject) override;
	virtual void RemoveRealtimeDemander(SObject* InObject) override;

	virtual void SetTitle(std::wstring_view InTitle) override;
	virtual std::wstring GetTitle() override;

	virtual IRHIFactory* GetFactory() override;
	virtual IPlatformKeyboard& GetPlatformKeyboard() override;
	virtual IPlatformMouse& GetPlatformMouse() override;
	// ~IApplicationInterface

	// IPlatformImageLoader
	virtual IPlatformImage* CreateImageFromFile(const std::filesystem::path& InAssetPath, int32 FrameIndex, ERHIPixelFormat PixelFormat) override;
	// ~IPlatformImageLoader

	HWND GetWindowHandle();

private:
	void ShrinkRealtimeDemanders();
	void UpdateRealtimeDemanders();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};