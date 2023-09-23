// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "SlateElementCommon.hlsli"

ConstantBuffer<NSlateRenderParams> PixelParameters : register(b0);
Texture2D Image : register(t0);
SamplerState Sampler : register(s0);

float4 main(in NFragment InFragment) : SV_TARGET
{
    //float4 FinalColor = Image.Sample(Sampler, InFragment.TexCoord);
    float4 FinalColor = 1.0f;
    return FinalColor;
}