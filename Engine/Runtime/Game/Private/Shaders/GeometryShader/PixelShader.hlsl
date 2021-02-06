// Copyright 2020 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"

GeometryPixel PS_Main(in Fragment inFrag)
{
	GeometryPixel oPx;
	oPx.Color = 1.0f;
	oPx.Normal = float4(normalize(inFrag.Normal), 1.0f);
	return oPx;
}