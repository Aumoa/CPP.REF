// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class ERHIResourceStates
{
	PRESENT = 0,
	RENDER_TARGET =				                                    0b100,
	COPY_DEST =										      0b100'0000'0000,
	COPY_SOURCE =									     0b1000'0000'0000,
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