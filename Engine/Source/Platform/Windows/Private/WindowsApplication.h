// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IApplicationInterface.h"
#include "WindowsIncludes.h"
#include "Multimedia/IPlatformImageLoader.h"

class SDXGIFactory;
class SWindowsPlatformKeyboard;
class SWindowsPlatformMouse;

class SWindowsApplication : implements SObject, implements IApplicationInterface, implements IPlatformImageLoader
{
	GENERATED_BODY(SWindowsApplication)

private:
	inline static SWindowsApplication* gApp;

	HINSTANCE hInstance = nullptr;
	HWND hWnd = nullptr;
	ETickMode TickMode = ETickMode::Realtime;
	ETickMode ActualTickMode = ETickMode::Realtime;
	std::vector<WeakPtr<SObject>> RealtimeDemanders;

	SPROPERTY(PlatformKeyboard)
	SObject* PlatformKeyboard = nullptr;
	SPROPERTY(PlatformMouse)
	SObject* PlatformMouse = nullptr;
	SPROPERTY(Factory)
	SDXGIFactory* Factory = nullptr;
	ComPtr<IWICImagingFactory> ImagingFactory;

public:
	SWindowsApplication(HINSTANCE hInstance);

	static int32 GuardedMain(std::span<const std::wstring> Argv);

	// IApplicationInterface
	virtual void Start() override;

	virtual Vector2N GetViewportSize() override;
	virtual float GetDpi() override;
	virtual void* GetWindowHandle() override;

	virtual void SetTickMode(ETickMode InTickMode) override;
	virtual ETickMode GetTickMode() override;
	virtual void AddRealtimeDemander(SObject* InObject) override;
	virtual void RemoveRealtimeDemander(SObject* InObject) override;

	virtual void SetTitle(std::wstring_view InTitle) override;
	virtual std::wstring GetTitle() override;

	virtual IRHIFactory* GetFactory() override;
	virtual IPlatformKeyboard& GetPlatformKeyboard() override;
	virtual IPlatformMouse& GetPlatformMouse() override;
	virtual IPlatformImageLoader& GetPlatformImageLoader() override;
	// ~IApplicationInterface

	// IPlatformImageLoader
	virtual IPlatformImage* CreateImageFromFile(const std::filesystem::path& InAssetPath, int32 FrameIndex, ERHIPixelFormat PixelFormat) override;
	virtual IPlatformImage* CreateImageFromBinary(std::span<const uint8> AssetsBin, int32 FrameIndex, ERHIPixelFormat PixelFormat) override;
	// ~IPlatformImageLoader

private:
	void ShrinkRealtimeDemanders();
	void UpdateRealtimeDemanders();
	ComPtr<IWICFormatConverter> DecodeImage(IWICBitmapDecoder* Decoder, int32 FrameIndex, ERHIPixelFormat PixelFormat);

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};