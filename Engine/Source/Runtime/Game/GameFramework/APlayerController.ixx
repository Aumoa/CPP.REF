// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:APlayerController;

import std.core;
import SC.Runtime.Core;
import :AController;

using namespace std;

/// <summary>
/// Represents controller that possess to pawn, process player input, etc...
/// </summary>
export class APlayerController : public AController
{
public:
	using Super = AController;

public:
	/// <summary>
	/// Initialize new <see cref="APlayerController"/> instance.
	/// </summary>
	/// <param name="name"> The controller name. </param>
	APlayerController(wstring_view name);
};