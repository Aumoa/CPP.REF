// Copyright 2020 Aumoa.lib. All right reserved.

#ifndef __GENERICLIGHTING_HLSLI__
#define __GENERICLIGHTING_HLSLI__

#include "ShaderCommon.hlsli"

#define LIGHT_TYPE_DIRECTIONAL 0

float3 ComputeDirectionalLight(Material material, Light light, float3 normal, float3 rayDir)
{
	float ambient = 0.0f, diffuse = 0.0f, specular = 0.0f;

	float3 direction = normalize(light.Ambiguous_01.xyz);
	float3 l = -direction;
	ambient = light.Ambient * material.Ambient;

	float diffuseFactor = dot(l, normal);
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(direction, normal);
		float specFactor = pow(max(dot(v, rayDir), 0.0f), max(material.SpecExp, 1.0f));

		diffuse = diffuseFactor * light.Diffuse * material.Diffuse;
		specular = specFactor * light.Specular * material.Specular;
	}

	return float3(ambient, diffuse, specular);
}

#endif