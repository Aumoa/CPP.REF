#if !defined( __GAMMAHELPER_HLSLI__ )

float4 ToGammaSpace( float4 linearSpaceColor, float gammaValue = 2.2f )
{
	return pow( linearSpaceColor, 1 / gammaValue );
}

float4 ToLinearSpace( float4 gammaSpaceColor, float gammaValue = 2.2f )
{
	return pow( gammaSpaceColor, gammaValue );
}

#endif