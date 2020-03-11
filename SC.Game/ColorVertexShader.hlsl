#include "Transform.hlsli"
#include "Camera.hlsli"
#include "Vertex.hlsli"

struct Fragment
{
	float4 PosH : SV_Position;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
};

ConstantBuffer<Camera> gCamera : register( b0 );
ConstantBuffer<Transform> gTransform : register( b1 );

StructuredBuffer<BoneTransform> gBoneTransform : register( t0 );
int gIsSkinned : register( b2 );

Fragment main( SkinnedVertex vIn )
{
	Fragment fg;
	fg.Color = vIn.Color;
	fg.Tex = vIn.Tex;

	if ( gIsSkinned )
	{
		float weights[4] = { vIn.Weights.x, vIn.Weights.y, vIn.Weights.z, 0 };
		weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

		uint indices[4] = { vIn.Indices.x, vIn.Indices.y, vIn.Indices.z, vIn.Indices.w };

		fg.PosH = 0;
		fg.Normal = 0;
		fg.Tangent = 0;

		[unroll]
		for ( uint i = 0; i < 4; ++i )
		{
			uint idx = indices[i];

			fg.PosH.xyz += mul( float4( vIn.Pos, 1.0f ), gBoneTransform[idx].World ).xyz * weights[i];
			fg.Normal += mul( vIn.Normal, ( float3x3 )gBoneTransform[idx].World ) * weights[i];
			fg.Tangent += mul( vIn.Tangent, ( float3x3 )gBoneTransform[idx].World ) * weights[i];
		}

		fg.PosH.w = 1.0f;
	}
	else
	{
		fg.PosH = float4( vIn.Pos, 1.0f );
	}
	fg.PosH = mul( fg.PosH, mul( gTransform.World, gCamera.ViewProj ) );
	fg.Normal = mul( vIn.Normal, ( float3x3 )gTransform.WorldInvTranspose );
	fg.Tangent = mul( vIn.Tangent, ( float3x3 )gTransform.World );

	return fg;
}