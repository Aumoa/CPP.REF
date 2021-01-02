// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class ERHIResourceFlags
{
	None = 0,
	AllowRenderTarget = 0b1
};

inline ERHIResourceFlags operator |(ERHIResourceFlags lh, ERHIResourceFlags rh)
{
	return (ERHIResourceFlags)((int32)lh | (int32)rh);
}

inline ERHIResourceFlags operator &(ERHIResourceFlags lh, ERHIResourceFlags rh)
{
	return (ERHIResourceFlags)((int32)lh & (int32)rh);
}

inline ERHIResourceFlags& operator |=(ERHIResourceFlags& lh, ERHIResourceFlags rh)
{
	lh = lh | rh;
	return lh;
}

inline ERHIResourceFlags& operator &=(ERHIResourceFlags& lh, ERHIResourceFlags rh)
{
	lh = lh & rh;
	return lh;
}

inline ERHIResourceFlags operator ~(ERHIResourceFlags unary)
{
	return (ERHIResourceFlags)(~(int32)unary);
}