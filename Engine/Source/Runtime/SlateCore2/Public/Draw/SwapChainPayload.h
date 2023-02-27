// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline SlateCore
{
	struct SwapChainPayload
	{
		enum class ECommands
		{
			Present,
		};

		IRHISwapChain* SwapChain;
		ECommands Commands;
	};
}