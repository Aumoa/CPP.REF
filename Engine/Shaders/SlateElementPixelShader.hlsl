// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "SlateElementCommon.hlsli"

Texture2D Image : register(t0);
SamplerState Sampler : register(s0);
ConstantBuffer<NSlateRenderParams> PixelParameters : register(b0);

float4 main(in NFragment InFragment) : SV_TARGET
{
    float4 FinalColor = Image.Sample(Sampler, InFragment.TexCoord);
    FinalColor *= PixelParameters.TintColor;
    FinalColor.a *= PixelParameters.RenderOpacity;
    return FinalColor;
}