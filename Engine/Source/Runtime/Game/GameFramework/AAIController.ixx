// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:AAIController;

import std.core;
import SC.Runtime.Core;
import :AController;

using namespace std;

/// <summary>
/// Represents controller that possess to pawn, process AI, etc...
/// </summary>
export class AAIController : public AController
{
public:
	using Super = AController;

public:
	/// <summary>
	/// Initialize new <see cref="AAIController"/> instance.
	/// </summary>
	AAIController();
};