// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IWindowView.h"

/// <summary>
/// Provide interface for platform specified application view.
/// </summary>
struct IFrameworkView : virtual public Object, virtual public IWindowView
{
	/// <summary>
	/// Start application.
	/// </summary>
	virtual void Start() = 0;

	/// <summary>
	/// Represents multicast delegate to be called every frame when the process is idle.
	/// </summary>
	using IdleDelegate = MulticastDelegate<void()>;

	/// <summary>
	/// Event that called every frame when the process is idle.
	/// </summary>
	MulticastDelegate<void()> Idle;

	/// <summary>
	/// Event that called when the application size change.
	/// </summary>
	MulticastDelegate<void(int32, int32)> Size;

	/// <summary>
	/// Get latest error code.
	/// </summary>
	virtual int32 GetLastError() const = 0;

	/// <summary>
	/// Set latest error code.
	/// </summary>
	virtual void SetLastError(int32 code) = 0;

	virtual int32 GetFrameworkWidth() const = 0;
	virtual int32 GetFrameworkHeight() const = 0;
};