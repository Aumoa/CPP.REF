#if !defined( __VERTEX_HLSLI__ )
#define __VERTEX_HLSLI__

struct Vertex
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
};

struct SkinnedVertex
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Weights : WEIGHTS;
	uint4 Indices : INDICES;
};

#endif