// Copyright 2020 Aumoa.lib. All right reserved.

struct Fragment
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
};

struct Pixel
{
	float4 Color : SV_TARGET;
};

Pixel PS_Main(in Fragment inFrag)
{
	Pixel oPx;
	oPx.Color = inFrag.Color;
	return oPx;
}