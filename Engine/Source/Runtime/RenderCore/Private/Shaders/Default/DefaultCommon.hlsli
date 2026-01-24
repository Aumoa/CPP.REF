#ifndef __DEFAULTCOMMON_HLSLI__
#define __DEFAULTCOMMON_HLSLI__

struct Payload
{
    float4 ColorAndDistance;
};

struct Attributes
{
    float2 Bary;
};

RWTexture2D<float4> u_OutputTexture : register(u0);
RaytracingAccelerationStructure t_Scene : register(t0);

#endif