// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Controller.h"

namespace libty::inline Game
{
	/// <summary>
	/// Represents controller that possess to pawn, process AI, etc...
	/// </summary>
	class GAME_API AAIController : extends(AController)
	{
		GENERATED_BODY(AAIController);

	public:
		/// <summary>
		/// Initialize new <see cref="AAIController"/> instance.
		/// </summary>
		AAIController();
	};
}