// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:AActor;

import std.core;
import SC.Runtime.Core;
import :GameObject;

using namespace std;

/// <summary>
/// Represents actor that, spawn to world and interaction with other actors.
/// </summary>
export class AActor : public GameObject
{
public:
	using Super = GameObject;

public:
	/// <summary>
	/// Initialize new <see cref="AActor"/> instance.
	/// </summary>
	/// <param name="name"> The actor name. </param>
	AActor(wstring_view name);
};