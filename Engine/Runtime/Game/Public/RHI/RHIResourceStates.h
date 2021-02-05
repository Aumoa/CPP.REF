// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class ERHIResourceStates
{
	PRESENT =                                                           0,
	VERTEX_AND_CONSTANT_BUFFER =                                      0b1,
	INDEX_BUFFER =                                	                 0b10,
	RENDER_TARGET =                                                 0b100,
	UNORDERED_ACCESS =                                             0b1000,
	DEPTH_WRITE =                                                 0b10000,
	DEPTH_READ =                                                 0b100000,
	NON_PIXEL_SHADER_RESOURCE =                                 0b1000000,
	PIXEL_SHADER_RESOURCE =                                    0b10000000,
	STREAM_OUT =                                              0b100000000,
	INDIRECT_ARGUMENT =                                      0b1000000000,
	COPY_DEST =                                             0b10000000000,
	COPY_SOURCE =                                          0b100000000000,
	RESOLVE_DEST =                                        0b1000000000000,
	RESOLVE_SOURCE =                                     0b10000000000000,
	
	COMMON =                                                            0,
	PREDICATION =                                            0b1000000000,
	RAYTRACING_ACCELERATION_STRUCTURE =         0b10000000000000000000000,
	SHADING_RATE_SOURCE =                     0b1000000000000000000000000,

	GenericRead = VERTEX_AND_CONSTANT_BUFFER | INDEX_BUFFER | NON_PIXEL_SHADER_RESOURCE | PIXEL_SHADER_RESOURCE | INDIRECT_ARGUMENT | COPY_SOURCE
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