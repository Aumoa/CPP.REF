// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "SlateElementCommon.hlsli"

static float4 RectVertices[] =
{
    float4(-1.0f, +1.0f, 0.0f, 1.0f),
    float4(+1.0f, +1.0f, 0.0f, 1.0f),
    float4(-1.0f, -1.0f, 0.0f, 1.0f),
    float4(+1.0f, -1.0f, 0.0f, 1.0f)
};

static float2 RectTexCoords[] =
{
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f),
    float2(0.0f, 1.0f),
    float2(1.0f, 1.0f)
};

Fragment main(uint InVertexId : SV_VertexID)
{
    Fragment F;
    F.Position = RectVertices[InVertexId];
    F.TexCoord = RectTexCoords[InVertexId];
    return F;
}