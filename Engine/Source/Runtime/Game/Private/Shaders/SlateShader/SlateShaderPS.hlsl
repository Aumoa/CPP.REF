// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SlateCommon.hlsli"

Pixel Main(in Fragment frag)
{
	Pixel px;
	//px.Color = gTexture.Sample(gSampler, frag.TexCoord);
	px.Color = 1.0f;
	return px;
}