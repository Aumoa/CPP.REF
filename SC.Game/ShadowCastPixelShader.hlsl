struct Fragment
{
	float4 PosH : SV_Position;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

struct Material_Constants
{
	int Index;
	int DiffuseMap;
	int NormalMap;
	int AlphaClip;
};

Texture2D<float4> gDiffuseMap0 : register( t0 );
Texture2D<float4> gDiffuseMap1 : register( t1 );
SamplerState gSampler : register( s0 );
ConstantBuffer<Material_Constants> gMaterial : register( b3 );

void main( Fragment pIn )
{
	float4 vertexColor = pIn.Color;
	float4 diffuse0 = gDiffuseMap0.Sample( gSampler, pIn.Tex );
	float4 color = lerp( vertexColor, diffuse0, ( float )min( gMaterial.DiffuseMap, 1 ) );

	if ( gMaterial.DiffuseMap == 2 )
	{
		color.xyz += gDiffuseMap1.Sample( gSampler, pIn.Tex ).xyz;
	}

	clip( color.a - 0.1f );
}