// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class ERHIResourceStates
{
	PRESENT = 0,
	RENDER_TARGET =				                                    0b100,
	UNORDERED_ACCESS =											   0b1000,
	DEPTH_WRITE =												  0b10000,
	DEPTH_READ =												 0b100000,
	COPY_DEST =										        0b10000000000,
	COPY_SOURCE =									       0b100000000000,
};

inline ERHIResourceStates operator |(ERHIResourceStates lh, ERHIResourceStates rh)
{
	return (ERHIResourceStates)((int32)lh | (int32)rh);
}

inline ERHIResourceStates operator &(ERHIResourceStates lh, ERHIResourceStates rh)
{
	return (ERHIResourceStates)((int32)lh & (int32)rh);
}

inline ERHIResourceStates& operator |=(ERHIResourceStates& lh, ERHIResourceStates rh)
{
	lh = lh | rh;
	return lh;
}

inline ERHIResourceStates& operator &=(ERHIResourceStates& lh, ERHIResourceStates rh)
{
	lh = lh & rh;
	return lh;
}

inline ERHIResourceStates operator ~(ERHIResourceStates unary)
{
	return (ERHIResourceStates)(~(int32)unary);
}