// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:IWindowView;

import SC.Runtime.Core;

export struct IWindowView : virtual public Object
{
	/// <summary>
	/// Get window handle that platform specialized.
	/// </summary>
	virtual void* GetWindowHandle() const = 0;
};