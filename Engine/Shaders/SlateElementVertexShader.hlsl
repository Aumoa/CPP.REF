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
StructuredBuffer<NSlatePaintGeometry> PaintGeometry : register(t0);

NFragment main(uint InVertexId : SV_VertexID, uint InInstanceId : SV_InstanceID)
{
    float2 V = RectVertices[InVertexId];
    
    // make element space.
    V = V * PaintGeometry[InInstanceId].LocalSize;
    V = mul(V, float2x2(PaintGeometry[InInstanceId].Transformation));
    V = V + PaintGeometry[InInstanceId].Translation;
    
    // make center based space.
    float2 HalfScreen = ScreenInfo.ScreenResolution * 0.5f;
    V = V - HalfScreen;
    
    // make to HDC.
    V = V / HalfScreen;
    V = V * float2(+1.0f, -1.0f);
    
    float2 T = RectTexCoords[InVertexId];
    float2 S = PaintGeometry[InInstanceId].TextureCoordinate.zw - PaintGeometry[InInstanceId].TextureCoordinate.xy;
    T = T * S + PaintGeometry[InInstanceId].TextureCoordinate.xy;
    
    NFragment F;
    //F.Position = float4(RectVertices[InVertexId], 0, 1.0f);
    F.Position = float4(V, 0, 1.0f);
    F.TexCoord = T;
    F.InstanceID = InInstanceId;
    return F;
}