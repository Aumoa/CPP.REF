namespace CommonHLSL
{
	const char CommonTypes[] = R"(
#ifndef __COMMONTYPES_HLSLI__
#define __COMMONTYPES_HLSLI__

struct Vertex
{
	float3 Position : POSITION;
};

struct Fragment
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

struct Pixel
{
	float4 Color : SV_TARGET0;
};

#endif
)";
}