Texture2D<float4> gHDRBuffer : register( t0 );
RWTexture2D<float4> gOutput : register( u0 );

static uint2 indexes[4] =
{
	uint2( 0, 0 ),
	uint2( 1, 0 ),
	uint2( 0, 1 ),
	uint2( 1, 1 )
};

[numthreads( 32, 32, 1 )]
void main( uint3 dispatchThreadID : SV_DispatchThreadID )
{
	float4 samples = 0;
	[unroll] for ( uint i = 0; i < 4; ++i )
	{
		samples += gHDRBuffer[dispatchThreadID.xy * 2 + indexes[i]];
	}

	float4 color = samples * 0.25f;

	gOutput[dispatchThreadID.xy] = color;
}