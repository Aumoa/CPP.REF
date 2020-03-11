struct Fragment
{
	float4 PosH : SV_Position;
	float2 Tex : TEXCOORD;
};

struct Pixel
{
	float4 Color : SV_Target0;
};

Texture2D<float4> gSkybox : register( t0 );
SamplerState gSkyboxSampler : register( s0 );

Pixel main( Fragment frag )
{
	Pixel px;
	px.Color = gSkybox.Sample( gSkyboxSampler, frag.Tex );
	return px;
}