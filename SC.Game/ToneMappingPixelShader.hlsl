#include "HDR.hlsli"
#include "GammaHelper.hlsli"

struct Fragment
{
	float4 PosH : SV_Position;
	float2 Tex : TEXCOORD;
};

struct Pixel
{
	float4 Color : SV_Target0;
};

Texture2D<float4> gHDRSource : register( t0 );
SamplerState gSampler : register( s0 );
ConstantBuffer<HDRConstants> gHDRConstants : register( b0 );

float4 ToneMapping( float4 Color, float AvgLum, float GammaValue = 2.2f, float GammaInv = 2.2f )
{
	float MiddleGrey = 1.5f;
	float LumWhiteSqr = 3.0f;

	float LScale = CalcLum( Color.xyz );
	LScale *= MiddleGrey / AvgLum;
	LScale = ( LScale + LScale * LScale / LumWhiteSqr ) / ( 1.0f + LScale );

	float3 color = pow( Color.xyz, GammaValue );
	color.xyz = color.xyz * LScale;
	color = pow( color, GammaInv );
	return float4( color, Color.w );
}

Pixel main( Fragment pIn )
{
	Pixel px;

	px.Color = gHDRSource.Sample( gSampler, pIn.Tex );
	if ( px.Color.w < 0.5f )
		px.Color = ToneMapping( px.Color, gHDRConstants.AverageLum, 1.0f, 1.0f );

	return px;
}