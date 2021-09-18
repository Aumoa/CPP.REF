// Copyright 2020-2021 Aumoa.lib. All right reserved.

#ifndef __SLATE_COMMON_HLSLI__
#define __SLATE_COMMON_HLSLI__

struct Fragment
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

struct Pixel
{
    float4 Color : SV_TARGET0;
};

struct SlateConstants
{
    float2 ScreenSize;
};

struct SlateElement
{
    float2x2 M;
    float2 AbsolutePosition;
    float2 AbsoluteSize;
    float2 TexturePosition;
    float2 TextureSize;
    float Depth;
    float3 pad;
};

Fragment GetSlateFragment(SlateConstants constants, SlateElement element, float2 vertexPos, float2 texturePos)
{
    Fragment frag;

    float2 slatePos = vertexPos;
    slatePos *= element.AbsoluteSize;
    slatePos = mul(element.M, slatePos);
    slatePos += element.AbsolutePosition;

    float2 ndc = slatePos;
    ndc /= constants.ScreenSize * 0.5f;
    ndc -= 1.0f;
    ndc.y = -ndc.y;

    frag.Position = float4(float3(ndc, element.Depth), 1.0f);
    frag.TexCoord = texturePos * element.TextureSize + element.TexturePosition;

    return frag;
}

#endif