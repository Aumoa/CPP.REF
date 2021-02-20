// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"

QuadFrag VS_Main(in uint inVertexId : SV_VERTEXID)
{
	QuadFrag oFrag;
	oFrag.PosH = QuadPos[inVertexId];
	oFrag.Tex = QuadTex[inVertexId];
	return oFrag;
}