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

struct Vertex
{
    float3 Position : POSITION;
    float4 Color : COLOR;
};

struct Fragment
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

struct Pixel
{
    float4 Color : SV_TARGET;
};

#ifdef __VERTEX_SHADER__

cbuffer CameraBuffer : register(b0)
{
    row_major matrix ViewProjection;
};

#endif

#endif