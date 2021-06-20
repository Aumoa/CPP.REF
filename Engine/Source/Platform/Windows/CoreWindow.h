// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "IFrameworkView.h"

/// <summary>
/// Represents the single window with input events and basic user interface behaviors.
/// </summary>
class CoreWindow : virtual public Object, virtual public IFrameworkView
{
public:
	using Super = Object;

private:
	void* _hwnd = nullptr;
	uint8 _bMainLoop : 1 = false;
	int32 _lastError = 0;

public:
	/// <summary>
	/// Initialize new <see cref="CoreWindow"/> instance.
	/// </summary>
	CoreWindow();

	/// <inheritdoc/>
	virtual void Start() override;

	/// <inheritdoc/>
	virtual void* GetWindowHandle() const override;

	/// <inheritdoc/>
	virtual int32 GetLastError() const override;

	/// <inheritdoc/>
	virtual void SetLastError(int32 code) override;
};