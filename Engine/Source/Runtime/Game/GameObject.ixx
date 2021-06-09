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
	mutable wstring _name;

public:
	/// <summary>
	/// Initialize new <see cref="GameObject"/> instance.
	/// </summary>
	/// <param name="name"> The object name. </param>
	GameObject();

	/// <inheritdoc/>
	virtual wstring ToString() const;

	/// <summary>
	/// Set object name to represents this object.
	/// </summary>
	void SetName(wstring_view value) { _name = value; }

	/// <summary>
	/// Get object name to represents this object.
	/// </summary>
	wstring GetName() const;
};