// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IFrameworkView.h"
#include <Windows.h>

/// <summary>
/// Represents the single window with input events and basic user interface behaviors.
/// </summary>
class SCoreWindow : virtual public SObject, virtual public IFrameworkView
{
	GENERATED_BODY(SCoreWindow)

public:
	static constexpr wchar_t ApplicationTitle[] = L"GameApp";

private:
	HWND _hwnd = nullptr;
	uint8 _bMainLoop : 1 = false;
	int32 _lastError = 0;
	ETickMode _tickMode = ETickMode::Realtime;

public:
	/// <summary>
	/// Initialize new <see cref="SCoreWindow"/> instance.
	/// </summary>
	SCoreWindow();

	/// <inheritdoc/>
	virtual void Start() override;

	/// <inheritdoc/>
	virtual void* GetWindowHandle() const override;

	/// <inheritdoc/>
	virtual int32 GetLastError() const override;

	/// <inheritdoc/>
	virtual void SetLastError(int32 code) override;

	/// <inheritdoc/>
	virtual Vector2N GetFrameworkSize() const override;

	/// <inheritdoc/>
	virtual void SetFrameworkTitle(const std::wstring& frameworkTitle) override;

	/// <inheritdoc/>
	virtual void SetTickMode(ETickMode tickMode) override;

	/// <inheritdoc/>
	virtual ETickMode GetTickMode() const override;

	/// <inheritdoc/>
	virtual void Present() override;

private:
	// Functions for internal only.
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void SetupHwndParameters(HWND hWnd, LPARAM lParam);
	static SCoreWindow* GetThis(HWND hWnd);
	static void FinalizeHwndParameters(HWND hWnd);
};