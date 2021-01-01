// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class RHIResourceStates
{
	PRESENT = 0,
	RENDER_TARGET =				                                    0b100,
	COPY_DEST =										      0b100'0000'0000,
	COPY_SOURCE =									     0b1000'0000'0000,
};

inline RHIResourceStates operator |(RHIResourceStates lh, RHIResourceStates rh)
{
	return (RHIResourceStates)((int32)lh | (int32)rh);
}

inline RHIResourceStates operator &(RHIResourceStates lh, RHIResourceStates rh)
{
	return (RHIResourceStates)((int32)lh & (int32)rh);
}

inline RHIResourceStates& operator |=(RHIResourceStates& lh, RHIResourceStates rh)
{
	lh = lh | rh;
	return lh;
}

inline RHIResourceStates& operator &=(RHIResourceStates& lh, RHIResourceStates rh)
{
	lh = lh & rh;
	return lh;
}

inline RHIResourceStates operator ~(RHIResourceStates unary)
{
	return (RHIResourceStates)(~(int32)unary);
}