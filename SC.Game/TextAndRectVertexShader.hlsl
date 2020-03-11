struct Fragment
{
	float4 PosH : SV_Position;
	float2 Tex : TEXCOORD;
};

struct tag_Resolution
{
	float width;
	float height;
};

struct tag_Rect
{
	float left;
	float top;
	float width;
	float height;
};

struct tag_ShaderInfo
{
	tag_Rect input;
	tag_Rect output;
};

static float2 tex[4] =
{
	float2( 0.0f, 0.0f ),
	float2( 1.0f, 0.0f ),
	float2( 0.0f, 1.0f ),
	float2( 1.0f, 1.0f )
};

ConstantBuffer<tag_Resolution> gResolution : register( b0 );
StructuredBuffer<tag_ShaderInfo> gShaderInfo : register( t0 );

Fragment main( uint vertexId : SV_VertexID, uint instanceId : SV_InstanceID )
{
	Fragment fg;

	tag_ShaderInfo shaderInfo = gShaderInfo[instanceId];
	tag_Rect input = shaderInfo.input;
	tag_Rect output = shaderInfo.output;

	fg.PosH = float4( tex[vertexId] * float2( output.width, output.height ) + float2( output.left, output.top ), 0.0f, 1.0f );
	fg.PosH.xy /= float2( gResolution.width, gResolution.height ) * 0.5f;
	fg.PosH.xy -= 1.0f;
	fg.PosH.y = -fg.PosH.y;

	fg.Tex = tex[vertexId];
	fg.Tex *= float2( input.width, input.height );
	fg.Tex += float2( input.left, input.top );

	return fg;
}