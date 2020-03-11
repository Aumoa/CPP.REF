#if !defined( __HDR_HLSLI__ )
#define __HDR_HLSLI__

struct HDRConstants
{
	float AverageLum;
};

float CalcLum( float3 value )
{
	const float3 lum = float3( 0.299f, 0.587f, 0.114f );
	return dot( value, lum );
}

#endif