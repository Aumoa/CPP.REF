// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:GameObject;

import std.core;
import SC.Runtime.Core;

using namespace std;

/// <summary>
/// Represents all classes in the smart component game framework hierarchy and provides low-level services to derived class.
/// </summary>
export class GameObject : virtual public Object
{
public:
	using Super = Object;

private:
	wstring _name;

public:
	/// <summary>
	/// Initialize new <see cref="GameObject"/> instance.
	/// </summary>
	/// <param name="name"> The object name. </param>
	GameObject(wstring_view name);

	/// <inheritdoc/>
	virtual wstring ToString() const;

	/// <summary>
	/// Get object name to represents this object.
	/// </summary>
	/// <returns></returns>
	wstring GetName() const;
};