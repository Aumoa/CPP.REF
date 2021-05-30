// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:GameInstance;

import std.core;
import SC.Runtime.Core;
import :GameObject;

/// <summary>
/// Represents single game instance while application are running.
/// </summary>
export class GameInstance : public GameObject
{
public:
	using Super = GameObject;

public:
	/// <summary>
	/// Initialize new <see cref="GameInstance"/> instance.
	/// </summary>
	/// <param name="name"> The instance name. </param>
	GameInstance(std::wstring_view name) : Super(name)
	{
	}

	/// <summary>
	/// Run the application.
	/// </summary>
	/// <returns> Return primary exit code. </returns>
	int32 Run()
	{
		return 0;
	}
};