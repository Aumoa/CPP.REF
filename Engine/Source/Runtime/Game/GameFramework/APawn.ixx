// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:APawn;

import std.core;
import :AActor;

using namespace std;

/// <summary>
/// Represents pawn that controller can be possessed.
/// </summary>
export class APawn : public AActor
{
public:
	using Super = AActor;

public:
	/// <summary>
	/// Initialize new <see cref="APawn"/> instance.
	/// </summary>
	/// <param name="name"> The pawn name. </param>
	APawn(wstring_view name);
};