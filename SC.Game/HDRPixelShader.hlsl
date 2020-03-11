#include "Camera.hlsli"
#include "Light.hlsli"
#include "Material.hlsli"

struct Fragment
{
	float4 PosH : SV_Position;
	float2 Tex : TEXCOORD;
};

struct Pixel
{
	float4 Color : SV_Target0;
};

Texture2D<float4> gNormalBuffer : register( t0 );
Texture2D<uint> gMaterialIndexBuffer : register( t1 );
Texture2D<float1> gDepthBuffer : register( t2 );
SamplerState gSampler : register( s0 );
SamplerComparisonState gSamplerComparison : register( s1 );

ConstantBuffer<Camera> gCamera : register( b0 );
ConstantBuffer<Light> gLight : register( b1 );
StructuredBuffer<Material> gMaterials : register( t3 );

Texture2D<float> gShadowDepth : register( t4 );

float3 ViewPosFromDepth( float2 tex, float depth )
{
	float z = depth;

	float4 clipSpacePosition = float4( tex * 2.0f - 1.0f, z, 1.0f );
	clipSpacePosition.y = -clipSpacePosition.y;

	float4 viewSpacePosition = mul( clipSpacePosition, gCamera.ProjInv );
	viewSpacePosition /= viewSpacePosition.w;

	return viewSpacePosition.xyz;
}

float3 WorldPosFromViewPos( float3 viewPos )
{
	float4 worldSpacePosition = mul( float4( viewPos, 1.0f ), gCamera.ViewInv );

	return worldSpacePosition.xyz;
}

uint Sample( Texture2D<uint> value, float2 tex )
{
	uint width, height;
	value.GetDimensions( width, height );

	uint x = ( uint )( tex.x * ( float )( width - 1 ) );
	uint y = ( uint )( tex.y * ( float )( height - 1 ) );

	return value[uint2( x, y )];
}

Pixel main( Fragment pIn )
{
	Pixel px;

	float4 sampled = gNormalBuffer.Sample( gSampler, pIn.Tex );
	float3 normal = sampled.xyz;

	if ( sampled.w < 0.5f )
	{
		px.Color = float4( ( float3 )1.0f, 1.0f );
	}
	else
	{
		float3 light = 0;

		float depth = gDepthBuffer.Sample( gSampler, pIn.Tex );
		float3 viewPos = ViewPosFromDepth( pIn.Tex, depth );
		float3 worldPos = WorldPosFromViewPos( viewPos );
		uint matIdx = Sample( gMaterialIndexBuffer, pIn.Tex );

		float3 ray = gCamera.Pos - worldPos;
		float3 rayDir = normalize( ray );

		switch ( gLight.Type )
		{
		case 0:  // DirectionalLight
			light += ComputeDirectionalLight( gMaterials[matIdx], gLight, normal, rayDir );
			break;
		}

		if ( gLight.ShadowCast )
		{
			float4 lightPos = mul( float4( worldPos, 1.0f ), gLight.ViewProj );
			lightPos.xyzw /= lightPos.w;

			uint wid, hei;
			gShadowDepth.GetDimensions( wid, hei );

			float cellx = 1.0f / ( float )wid * 0.5f;
			float celly = 1.0f / ( float )hei * 0.5f;

			float2 samples[4] =
			{
				float2( -cellx, -celly ), float2( cellx, -celly ),
				float2( -cellx, celly ), float2( cellx, celly )
			};

			float2 samplePos = ( float2( lightPos.x, -lightPos.y ) + 1.0f ) * 0.5f;
			float lightdepth = 0.0f;
			[unroll]
			for ( int i = 0; i < 4; ++i )
			{
				lightdepth += gShadowDepth.SampleCmpLevelZero( gSamplerComparison, samplePos + samples[i], lightPos.z );
			}
			lightdepth *= 0.25f;

			light.yz *= lightdepth;
		}

		px.Color = float4( ( light.x + light.y ) * gLight.Color + light.z, 0.0f );
	}

	return px;
}