#if !defined( __LIGHT_HLSLI__ )
#define __LIGHT_HLSLI__

#include "Material.hlsli"

struct Light
{
	uint Type;
	float3 Color;
	float Ambient;
	float Diffuse;
	float Specular;
	int ShadowCast;
	row_major matrix ViewProj;
	float4 Ambiguous_01;
};

float3 ComputeDirectionalLight( Material material, Light light, float3 normal, float3 rayDir )
{
	float ambient = 0.0f, diffuse = 0.0f, specular = 0.0f;

	float3 direction = normalize( light.Ambiguous_01.xyz );
	float3 l = -direction;
	ambient = light.Ambient * material.Ambient;

	float diffuseFactor = dot( l, normal );
	[flatten]
	if ( diffuseFactor > 0.0f )
	{
		float3 v = reflect( direction, normal );
		float specFactor = pow( max( dot( v, rayDir ), 0.0f ), max( 1.0f, material.SpecExp ) );

		diffuse = diffuseFactor * light.Diffuse * material.Diffuse;
		specular = specFactor * light.Specular * material.Specular;
	}

	return float3( ambient, diffuse, specular );
}

#endif