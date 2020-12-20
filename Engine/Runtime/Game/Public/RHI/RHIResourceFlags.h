// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::RHI
{
	enum class RHIResourceFlags
	{
		None = 0,
		AllowRenderTarget = 0b1
	};

	inline RHIResourceFlags operator |(RHIResourceFlags lh, RHIResourceFlags rh)
	{
		return (RHIResourceFlags)((int32)lh | (int32)rh);
	}

	inline RHIResourceFlags operator &(RHIResourceFlags lh, RHIResourceFlags rh)
	{
		return (RHIResourceFlags)((int32)lh & (int32)rh);
	}

	inline RHIResourceFlags& operator |=(RHIResourceFlags& lh, RHIResourceFlags rh)
	{
		lh = lh | rh;
		return lh;
	}

	inline RHIResourceFlags& operator &=(RHIResourceFlags& lh, RHIResourceFlags rh)
	{
		lh = lh & rh;
		return lh;
	}

	inline RHIResourceFlags operator ~(RHIResourceFlags unary)
	{
		return (RHIResourceFlags)(~(int32)unary);
	}
}