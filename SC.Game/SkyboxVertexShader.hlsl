#include "Vertex.hlsli"
#include "Camera.hlsli"

struct Fragment
{
	float4 PosH : SV_Position;
	float2 Tex : TEXCOORD;
};

ConstantBuffer<Camera> gCamera : register( b0 );

Fragment main( Vertex vIn, uint vertexId : SV_VertexID )
{
	Fragment fg;
	fg.PosH = mul( float4( vIn.Pos + gCamera.Pos, 1.0f ), gCamera.ViewProj );
	fg.PosH.z = fg.PosH.w;
	fg.Tex = vIn.Tex;
	return fg;
}