struct Fragment
{
	float4 PosH : SV_Position;
	float2 Tex : TEXCOORD;
};

struct Pixel
{
	float4 Color : SV_Target0;
};

Texture2D<float4> gColorBuffer : register( t0 );
SamplerState gSampler : register( s0 );

Pixel main( Fragment pIn )
{
	Pixel px;

	float4 color = gColorBuffer.Sample( gSampler, pIn.Tex );
	px.Color = color;

	return px;
}