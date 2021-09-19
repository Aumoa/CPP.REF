// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IWindowView.h"

/// <summary>
/// Provide interface for platform specified application view.
/// </summary>
struct IFrameworkView : virtual public SObject, virtual public IWindowView
{
	enum class ETickMode
	{
		Realtime,
		Ontime,
	};

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
	DECLARE_MULTICAST_DELEGATE(IdleEvent);
	IdleEvent Idle;

	/// <summary>
	/// Event that called when the application size change.
	/// </summary>
	DECLARE_MULTICAST_DELEGATE(SizeEvent, int32, int32);
	SizeEvent Size;

	/// <summary>
	/// Get latest error code.
	/// </summary>
	virtual int32 GetLastError() const = 0;

	/// <summary>
	/// Set latest error code.
	/// </summary>
	virtual void SetLastError(int32 code) = 0;

	/// <summary>
	/// Get framework size.
	/// </summary>
	virtual Vector2N GetFrameworkSize() const = 0;

	/// <summary>
	/// Set framework title.
	/// </summary>
	virtual void SetFrameworkTitle(const std::wstring& frameworkTitle) = 0;

	/// <summary>
	/// Set tick mode.
	/// </summary>
	virtual void SetTickMode(ETickMode tickMode) = 0;

	/// <summary>
	/// Get tick mode.
	/// </summary>
	virtual ETickMode GetTickMode() const = 0;

	/// <summary>
	/// Present for ETickMode::Ontime.
	/// </summary>
	virtual void Present() = 0;
};