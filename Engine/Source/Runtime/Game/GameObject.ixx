// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:GameObject;

import std.core;
import SC.Runtime.Core;

/// <summary>
/// Represents all classes in the smart component game framework hierarchy and provides low-level services to derived class.
/// </summary>
export class GameObject : virtual public Object
{
public:
	using Super = Object;

private:
	std::wstring _name;

public:
	/// <summary>
	/// Initialize new <see cref="GameObject"/> instance.
	/// </summary>
	/// <param name="name"> The object name. </param>
	GameObject(std::wstring_view name) : Super()
		, _name(name)
	{
	}

	/// <summary>
	/// Get object name to represent this object.
	/// </summary>
	/// <returns> The string view instance. </returns>
	virtual std::wstring ToString() const
	{
		return _name;
	}
};