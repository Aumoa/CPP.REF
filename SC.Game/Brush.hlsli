#if !defined( __BRUSH_HLSLI__ )
#define __BRUSH_HLSLI__

#include "Common.hlsli"

#define SOLID_COLOR_BRUSH 0
#define REVEAL_BRUSH 1

struct Brush
{
	int Type;
	float Opacity;
	float2 padding0;
	float4 Attribute0;
	float4 Attribute1;
};

float4 GetBrushColor( Brush brush, Resolution res, Resolution cursor, float2 pos )
{
	if ( brush.Type == SOLID_COLOR_BRUSH )
	{
		return brush.Attribute0 * float4( 1.0f, 1.0f, 1.0f, brush.Opacity );
	}
	else if ( brush.Type == REVEAL_BRUSH )
	{
		float alpha = saturate( 1.0f - distance( pos, float2( cursor.Width, cursor.Height ) ) / brush.Attribute1.x );
		alpha = clamp( alpha, brush.Attribute1.y, 1.0f );

		return brush.Attribute0 * float4( 1.0f, 1.0f, 1.0f, alpha * brush.Opacity );
	}
	else
	{
		return 0;
	}
}

#endif