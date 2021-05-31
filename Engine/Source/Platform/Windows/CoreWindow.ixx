// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Platform.Windows:CoreWindow;

import SC.Runtime.Core;

/// <summary>
/// Represents the single window with input events and basic user interface behaviors.
/// </summary>
export class CoreWindow : Object
{
public:
	using Super = Object;

private:
	void* _hwnd = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="CoreWindow"/> instance.
	/// </summary>
	CoreWindow() : Super()
	{
		InternalCreateWindow();
	}

private:
	void InternalCreateWindow();
};