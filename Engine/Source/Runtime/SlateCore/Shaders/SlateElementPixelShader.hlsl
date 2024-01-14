// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "SlateElementCommon.hlsli"

Texture2D Image : register(t0);
SamplerState Sampler : register(s0);
StructuredBuffer<NSlateRenderParams> PixelParameters : register(t1);

float4 main(in NFragment InFragment) : SV_TARGET
{
    float4 SampleColor = 1.0f;
    
    bool bHasTexture = (PixelParameters[InFragment.InstanceID].RenderStates & RS_HasTexture) > 0;
    [flatten]
    if (bHasTexture)
    {
        SampleColor = Image.Sample(Sampler, InFragment.TexCoord);
    }
    
    bool bAlphaOnly = (PixelParameters[InFragment.InstanceID].RenderStates & RS_AlphaOnly) > 0;
    [flatten]
    if (bAlphaOnly)
    {
        SampleColor.xyz = SampleColor.a;
    }
    
    float4 FinalColor = SampleColor * PixelParameters[InFragment.InstanceID].TintColor;
    FinalColor.a *= PixelParameters[InFragment.InstanceID].RenderOpacity;
    return FinalColor;
}