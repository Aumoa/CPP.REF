// Copyright 2020-2023 Aumoa.lib. All right reserved.

#ifndef __SLATE_ELEMENT_COMMON_HLSLI__
#define __SLATE_ELEMENT_COMMON_HLSLI__

struct NFragment
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
};

struct NSlatePaintGeometry
{
    float4 Transformation;
    float2 Translation;
    float2 LocalSize;
    float4 TextureCoordinate;
};

struct NSlateRenderParams
{
    float4 TintColor;
    float RenderOpacity;
    int RenderStates;
};

struct NSlateScreenInformation
{
    float2 ScreenResolution;
};

#define RS_HasTexture 0x1
#define RS_AlphaOnly 0x2

#endif