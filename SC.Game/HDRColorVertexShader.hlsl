struct Fragment
{
	float4 PosH : SV_Position;
	float2 Tex : TEXCOORD;
};

static float4 pos[4] =
{
	float4( -1.0f, +1.0f, 0.0f, 1.0f ),
	float4( +1.0f, +1.0f, 0.0f, 1.0f ),
	float4( -1.0f, -1.0f, 0.0f, 1.0f ),
	float4( +1.0f, -1.0f, 0.0f, 1.0f )
};

static float2 tex[4] =
{
	float2( 0.0f, 0.0f ),
	float2( 1.0f, 0.0f ),
	float2( 0.0f, 1.0f ),
	float2( 1.0f, 1.0f )
};

Fragment main( uint vertexId : SV_VertexID )
{
	Fragment fg;
	fg.PosH = pos[vertexId];
	fg.Tex = tex[vertexId];
	return fg;
}