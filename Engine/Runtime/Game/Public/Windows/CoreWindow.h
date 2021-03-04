// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "HandleBase.h"

class Application;

class GAME_API CoreWindow : public HandleBase
{
public:
	using Super = HandleBase;

	using CreatedDelegate = TMulticastDelegate<void()>;
	using DestroyDelegate = TMulticastDelegate<void()>;
	using SizingDelegate = TMulticastDelegate<void(int32, int32)>;

private:
	static constexpr const wchar_t* ClassName = L"CoreWindow";
	static std::map<HWND, CoreWindow*> coreWindows;

	Application* app;
	HWND hWnd;
	bool bInit : 1;

protected:
	bool bPostQuitMessage : 1;

public:
	CoreWindow(Application* app);
	~CoreWindow() override;
	
	virtual void Init();
	void Close() override;

	vs_property(bool, IsVisible);
	bool IsVisible_get() const;
	void IsVisible_set(bool value);
	vs_property(TRefPtr<String>, Title);
	TRefPtr<String> Title_get() const;
	void Title_set(TRefPtr<String> value);
	vs_property_get_auto(HWND, HWnd, hWnd);
	vs_property_get_auto(bool, IsInitialized, bInit);

public:
	CreatedDelegate Created;
	DestroyDelegate Destroy;
	SizingDelegate Sizing;
	SizingDelegate PostSized;

private:
	static void Internal_RegisterClass(Application* app_internal);
	void Internal_CreateWindow();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void PostInitializeCreateWindow(HWND hWnd, LPARAM lParam);
	static void PostUninitializeWindow(HWND hWnd);
	static CoreWindow* GetCoreWindow(HWND hWnd);

private:
	void OnDestroy();
};