namespace ColorShaderHLSL
{
	const char VertexShader[] = R"(
#include "CommonHLSL/CommonTypes.hlsli"

static float4 gPos[] =
{
	float4(-1.0f, +1.0f, 0.0f, 1.0f),
	float4(+1.0f, +1.0f, 0.0f, 1.0f),
	float4(-1.0f, -1.0f, 0.0f, 1.0f)
};

void VSMain(in uint iVertexId : SV_VERTEXID, out Fragment vOut)
{
	vOut.Position = gPos[iVertexId];
	vOut.Color = (float4)1.0f;
}
)";
}