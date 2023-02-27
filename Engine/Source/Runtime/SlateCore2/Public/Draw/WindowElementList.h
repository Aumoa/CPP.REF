// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "SwapChainPayload.h"

namespace libty::inline SlateCore
{
	class WindowElementList
	{
		enum class EPayloadType : uint8
		{
			SwapChain
		};

	public:
		template<class T>
		T& CreatePayload()
		{
		}
	};
}