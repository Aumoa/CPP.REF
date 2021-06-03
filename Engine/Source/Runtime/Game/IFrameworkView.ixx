// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:IFrameworkView;

import SC.Runtime.Core;
import SC.Runtime.RenderCore;

/// <summary>
/// Provide interface for platform specified application view.
/// </summary>
export struct IFrameworkView : virtual public Object, virtual public IWindowView
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
	/// To be called every frame when the process is idle.
	/// </summary>
	MulticastDelegate<void()> Idle;
};