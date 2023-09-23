// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "SlateElementCommon.hlsli"

static float2 RectVertices[] =
{
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f),
    float2(0.0f, 1.0f),
    float2(1.0f, 1.0f)
};

static float2 RectTexCoords[] =
{
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f),
    float2(0.0f, 1.0f),
    float2(1.0f, 1.0f)
};

ConstantBuffer<NSlateScreenInformation> ScreenInfo : register(b0);
ConstantBuffer<NSlatePaintGeometry> PaintGeometry : register(b1);

NFragment main(uint InVertexId : SV_VertexID)
{
    float2 V = RectVertices[InVertexId];
    
    // make element space.
    V = V * PaintGeometry.LocalSize;
    V = mul(V, float2x2(PaintGeometry.Transformation));
    V = V + PaintGeometry.Translation;
    
    // make center based space.
    float2 HalfScreen = ScreenInfo.ScreenResolution * 0.5f;
    V = V - HalfScreen;
    
    // make to HDC.
    V = V / HalfScreen;
    V = V * float2(+1.0f, -1.0f);
    
    NFragment F;
    F.Position = float4(V, 0, 1.0f);
    F.TexCoord = RectTexCoords[InVertexId];
    return F;
}