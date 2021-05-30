// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:IFrameworkView;

import SC.Runtime.Core;

/// <summary>
/// Provide interface for platform specified application view.
/// </summary>
export struct IFrameworkView : virtual public Object
{
	/// <summary>
	/// Start application.
	/// </summary>
	virtual void Start() = 0;
};