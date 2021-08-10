// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Controller.h"

/// <summary>
/// Represents controller that possess to pawn, process AI, etc...
/// </summary>
class GAME_API AAIController : public AController
{
public:
	using Super = AController;

public:
	/// <summary>
	/// Initialize new <see cref="AAIController"/> instance.
	/// </summary>
	AAIController();
};