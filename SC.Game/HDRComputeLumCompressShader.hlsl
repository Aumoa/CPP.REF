#include "HDR.hlsli"

Texture2D<float4> gHDRBuffer : register( t0 );
RWTexture2D<float4> gHalfHDRBuffer : register( u0 );
RWStructuredBuffer<float> gHDRConstants : register( u1 );

groupshared float gCache1[256];
groupshared float gCache2[16];

[numthreads( 256, 1, 1 )]
void main( uint3 tid : SV_DispatchThreadID )
{
	uint fullWidth;
	uint fullHeight;

	gHDRBuffer.GetDimensions( fullWidth, fullHeight );

	float avglum = 0;
	uint div = ( fullWidth / 2 - 1 ) / 256 + 1;
	float cut = 0;
	for ( uint i = 0; i < div; ++i )
	{
		int2 offset = int2( tid.x * div + i, tid.y );
		if ( offset.x < ( int )fullWidth && offset.y < ( int )fullHeight )
		{
			avglum += CalcLum( gHalfHDRBuffer[int2( tid.x * div + i, tid.y )].xyz );
			cut += 1.0f;
		}
	}
	gCache1[tid.x] = avglum / cut;

	GroupMemoryBarrierWithGroupSync();

	if ( tid.x < 16 )
	{
		avglum = 0;
		[unroll] for ( uint i = 0; i < 16; ++i )
		{
			avglum += gCache1[tid.x * 16 + i];
		}
		gCache2[tid.x] = avglum / 16.0f;
	}

	GroupMemoryBarrierWithGroupSync();

	if ( tid.x == 0 )
	{
		avglum = 0;
		[unroll] for ( uint i = 0; i < 16; ++i )
		{
			avglum += gCache2[i];
		}

		gHDRConstants[1 + tid.y] = avglum / 16.0f;
	}
}