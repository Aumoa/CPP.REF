#if !defined( __TRANSFORM_HLSLI__ )
#define __TRANSFORM_HLSLI__

struct Transform
{
	row_major matrix World;
	row_major matrix WorldInvTranspose;
};

struct BoneTransform
{
	row_major matrix World;
};

#endif