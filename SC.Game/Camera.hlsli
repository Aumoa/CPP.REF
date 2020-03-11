#if !defined( __CAMERA_HLSLI__ )
#define __CAMERA_HLSLI__

struct Camera
{
	row_major matrix ViewProj;
	row_major matrix ViewInv;
	row_major matrix ProjInv;
	float3 Pos;
};

#endif