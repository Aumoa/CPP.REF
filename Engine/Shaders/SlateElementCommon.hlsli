// Copyright 2020-2023 Aumoa.lib. All right reserved.

#ifndef __SLATE_ELEMENT_COMMON_HLSLI__
#define __SLATE_ELEMENT_COMMON_HLSLI__

struct Vertex
{
    float2 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

struct Fragment
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
};

#endif