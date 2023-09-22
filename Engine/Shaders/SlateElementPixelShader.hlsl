// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "SlateElementCommon.hlsli"

struct SlatePixelParameters
{
    float4 TintColor;
};

ConstantBuffer<SlatePixelParameters> PixelParameters : register(b0);
Texture2D Image : register(t0);
SamplerState Sampler : register(s0);

float4 main(in Fragment InFragment) : SV_TARGET
{
    float4 FinalColor = Image.Sample(Sampler, InFragment.TexCoord);
    return FinalColor;
}