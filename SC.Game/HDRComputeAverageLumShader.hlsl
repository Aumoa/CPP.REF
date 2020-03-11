#include "HDR.hlsli"

struct Timer
{
	float TimeDeltaInSeconds;
};

Texture2D<float4> gHDRBuffer : register( t0 );
RWTexture2D<float4> gHalfHDRBuffer : register( u0 );
RWStructuredBuffer<float> gHDRConstants : register( u1 );
ConstantBuffer<Timer> gTimerPack : register( b0 );

groupshared float gCache1[256];
groupshared float gCache2[16];

[numthreads( 1, 256, 1 )]
void main( uint3 tid : SV_DispatchThreadID )
{
	uint fullWidth;
	uint fullHeight;

	gHDRBuffer.GetDimensions( fullWidth, fullHeight );
	
	float avglum = 0;
	uint div = ( ( uint )fullWidth / 2 - 1 ) / 256 + 1;
	for ( uint i = 0; i < div; ++i )
	{
		if ( tid.y * div + i > fullHeight ) break;
		avglum += gHDRConstants[1 + tid.y * div + i];
	}
	gCache1[tid.y] = avglum / ( float )div;

	GroupMemoryBarrierWithGroupSync();

	if ( tid.y < 16 )
	{
		avglum = 0;
		for ( uint i = 0; i < 16; ++i )
		{
			avglum += gCache1[tid.y * 16 + i];
		}
		gCache2[tid.y] = avglum / 16.0f;
	}

	GroupMemoryBarrierWithGroupSync();

	if ( tid.y == 0 )
	{
		avglum = 0;
		for ( uint i = 0; i < 16; ++i )
		{
			avglum += gCache2[i];
		}
		avglum /= 16.0f;

		if ( distance( gHDRConstants[0], avglum ) > 1.0f )
		{
			gHDRConstants[0] = avglum;
		}
		else
		{
			gHDRConstants[0] = lerp( gHDRConstants[0], avglum, gTimerPack.TimeDeltaInSeconds );
		}
	}
}