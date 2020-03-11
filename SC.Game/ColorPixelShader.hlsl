#include "Camera.hlsli"

struct Fragment
{
	float4 PosH : SV_Position;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
};

struct Pixel
{
	float4 Color : SV_Target0;
	float4 Normal : SV_Target1;
	uint Material : SV_Target2;
};

struct Material_Constants
{
	int Index;
	int DiffuseMap;
	int NormalMap;
	int AlphaClip;
};

Texture2D<float4> gDiffuseMap0 : register( t0 );
Texture2D<float4> gDiffuseMap1 : register( t1 );
Texture2D<float4> gNormalMap0 : register( t2 );
ConstantBuffer<Material_Constants> gMaterial : register( b3 );
SamplerState gSampler : register( s0 );

float3 NormalSampleToWorldSpace( float3 SampledValue, float3 NormalW, float3 TangentW )
{
	float3 normalT = 2.0f * SampledValue - 1.0f;
	float3 T = normalize( TangentW - dot( TangentW, NormalW ) * NormalW );
	float3 B = cross( NormalW, T );
	return normalize( mul( normalT, float3x3( T, B, NormalW ) ) );
}

Pixel main( Fragment pIn )
{
	Pixel px;

	float4 vertexColor = pIn.Color;
	float4 diffuse0 = gDiffuseMap0.Sample( gSampler, pIn.Tex );
	float4 color = lerp( vertexColor, diffuse0, ( float )min( gMaterial.DiffuseMap, 1 ) );

	if ( gMaterial.DiffuseMap == 2 )
	{
		color.xyz += gDiffuseMap1.Sample( gSampler, pIn.Tex ).xyz;
	}

	clip( color.a - 0.1f );

	float3 normal = normalize( pIn.Normal );

	if ( gMaterial.NormalMap )
	{
		float3 bump = gNormalMap0.Sample( gSampler, pIn.Tex ).xyz;
		normal = NormalSampleToWorldSpace( bump, normal, normalize( pIn.Tangent ) );
	}

	px.Color = color;
	px.Normal = float4( normal, 1.0f );
	px.Material = gMaterial.Index;
	return px;
}