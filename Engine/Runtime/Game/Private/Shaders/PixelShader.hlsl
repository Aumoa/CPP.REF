// Copyright 2020 Aumoa.lib. All right reserved.

struct Fragment
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
};

struct Pixel
{
	float4 Color : SV_TARGET0;
	float4 Normal : SV_TARGET1;
};

Pixel PS_Main(in Fragment inFrag)
{
	Pixel oPx;
	oPx.Color = inFrag.Color;
	oPx.Normal = float4(normalize(inFrag.Normal), 1.0f);
	return oPx;
}