// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"

ConstantBuffer<ShaderCameraConstant> gCamera : register(b0);
RaytracingAccelerationStructure gSceneAS : register(t0);
StructuredBuffer<GeneralLight> gLights : register(t1);
SamplerState gSampler : register(s0);

RWTexture2D<float4> gColorOutput : register(u0);

inline float3 HitAttribute(float3 v1, float3 v2, float3 v3, float2 bary)
{
	return (v1 + bary.x * (v2 - v1) + bary.y * (v3 - v1));
}

inline float2 HitAttribute(float2 v1, float2 v2, float2 v3, float2 bary)
{
	return (v1 + bary.x * (v2 - v1) + bary.y * (v3 - v1));
}

inline RayDesc GenerateRay(uint2 launchIndex, float3 cameraPos, matrix projectionToWorld, float near = 0.01f, float far = 1000.0f)
{
	float2 xy = launchIndex + 0.5f; // center in the middle of the pixel.
	float2 screenPos = xy / DispatchRaysDimensions().xy * 2.0 - 1.0;

	// Invert Y for DirectX-style coordinates.
	screenPos.y = -screenPos.y;

	// Unproject the pixel coordinate into a ray.
	float4 world = mul(float4(screenPos, 0, 1), projectionToWorld);
	world.xyz /= world.w;

	RayDesc ray;
	ray.Origin = cameraPos;
	ray.Direction = normalize(world.xyz - ray.Origin);
	ray.TMin = near;
	ray.TMax = far;

	return ray;
}

inline int Payload_Type(inout Payload payload)
{
	return payload.Attributes & 0xFF;
}

inline void Payload_Type(inout Payload payload, int type)
{
	// Set attribute to zero.
	payload.Attributes = payload.Attributes & ~0xFF;
	// Assign attribute.
	payload.Attributes = payload.Attributes | (type & 0xFF);
}

inline int Payload_RecursiveCount(inout Payload payload)
{
	return (payload.Attributes & 0xFF00) >> 8;
}

inline void Payload_RecursiveCount(inout Payload payload, int count)
{
	// Set attribute to zero.
	payload.Attributes = payload.Attributes & ~0xFF00;
	// Assign attribute.
	payload.Attributes = payload.Attributes | ((count & 0xFF) << 8);
}

inline bool Payload_bShadowHit(inout Payload payload)
{
	return (payload.Attributes & 0x10000) != 0;
}

inline void Payload_bShadowHit(inout Payload payload, bool bHit)
{
	// Set attribute to zero.
	payload.Attributes = payload.Attributes & ~0x10000;
	// Assign attribute.
	payload.Attributes = payload.Attributes | (bHit << 16);
}

inline void TraceRaySimply(inout Payload payload, RayDesc rayDesc)
{
	int currRecursive = Payload_RecursiveCount(payload);
	if (currRecursive >= Payload_MaxRecursive)
	{
		return;
	}
	Payload_RecursiveCount(payload, currRecursive + 1);

	TraceRay(gSceneAS,
		RAY_FLAG_FORCE_OPAQUE,
		0xFF,
		0,
		1,
		0,
		rayDesc,
		payload
	);
}

[shader("raygeneration")]
void RayGeneration()
{
	uint2 launchIndex = (uint2)DispatchRaysIndex();

	Payload payload = (Payload)0;
	TraceRaySimply(payload, GenerateRay(launchIndex, gCamera.Pos, gCamera.ViewProjInv));
	gColorOutput[launchIndex] = float4(payload.Color);
}

[shader("miss")]
void Miss(inout Payload payload : SV_Payload)
{
	payload.Color = 0;
}

StructuredBuffer<Vertex> cVertexBuffer : register(t0, space2);
StructuredBuffer<uint> cIndexBuffer : register(t1, space2);
ConstantBuffer<RaytracingInstanceTransform> cTransform : register(b0, space2);

ConstantBuffer<Material> cMaterial : register(b1, space2);
Texture2D<float4> cDiffuseMap : register(t2, space2);
Texture2D<float4> cNormalMap : register(t3, space2);

inline float3 ComputeDirectionalLight(Material material, GeneralLight light, float3 normal, float3 rayDir, float3 posW)
{
	float3 ambient = 0, diffuse = 0, specular = 0;

	float3 direction = normalize(light.DirectionalLight_Direction);
	float3 l = -direction;

	// Calc basic ambient factor.
	ambient = light.Color * light.Ambient * material.Ambient;

	// Calc diffuse and specular factor.
	float diffuseFactor = dot(l, normal);
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		// Check shadow caster.
		RayDesc toLight = (RayDesc)0;
		toLight.Origin = posW;
		toLight.Direction = -light.DirectionalLight_Direction;
		toLight.TMin = 0.0001f;
		toLight.TMax = 1000.0f;

		Payload shadowPayload = (Payload)0;
		Payload_Type(shadowPayload, PayloadType_ShadowCast);
		TraceRaySimply(shadowPayload, toLight);

		if (!Payload_bShadowHit(shadowPayload))
		{
			float3 v = reflect(direction, normal);
			float specFactor = pow(max(dot(v, rayDir), 0.0f), max(material.SpecExp, 1.0f));

			diffuse = light.Color * diffuseFactor * light.Diffuse * material.Diffuse;
			specular = specFactor * light.Specular * material.Specular;
		}
	}

	// Make light color.
	return ambient + diffuse + specular;
}

inline float3 ComputePointLight(Material material, GeneralLight light, float3 normal, float3 rayDir, float3 posW)
{
	float3 ambient = 0, diffuse = 0, specular = 0;

	const float3 lightPos = light.PointLight_Position;

	float3 l = lightPos - posW;
	float len = length(l);
	l = l / len;
	float3 direction = -l;

	// Calc diffuse and specular factor.
	const float angleFactor = dot(l, normal);
	const float att = 1.0f /
		 (light.PointLight_Constant
		+ light.PointLight_Linear * len
		+ light.PointLight_Quad * len * len);

	// Calc basic ambient factor.
	ambient = light.Color * light.Ambient * material.Ambient * att;

	const float diffuseFactor = att;
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		// Check shadow caster.
		RayDesc toLight = (RayDesc)0;
		toLight.Origin = posW;
		toLight.Direction = l;
		toLight.TMin = 0.01f;
		toLight.TMax = len;

		Payload shadowPayload = (Payload)0;
		Payload_Type(shadowPayload, PayloadType_ShadowCast);
		TraceRaySimply(shadowPayload, toLight);

		if (!Payload_bShadowHit(shadowPayload))
		{
			float3 v = reflect(direction, normal);
			float specFactor = pow(max(dot(v, rayDir), 0.0f), max(material.SpecExp, 1.0f)) * att;

			diffuse = light.Color * diffuseFactor * light.Diffuse * material.Diffuse;
			specular = specFactor * light.Specular * material.Specular;
		}
	}

	// Make light color.
	return ambient + diffuse + specular;
}

inline float3 ComputeSpotLight(Material material, GeneralLight light, float3 normal, float3 rayDir, float3 posW)
{
	float3 ambient = 0, diffuse = 0, specular = 0;

	const float3 lightPos = light.SpotLight_Position;
	const float3 lightDir = light.SpotLight_Direction;

	float3 l = lightPos - posW;
	float len = length(l);
	l = l / len;
	float3 direction = -l;

	// Calc diffuse and specular factor.
	const float angleFactor = dot(l, normal);
	const float att = 1.0f /
		 (light.SpotLight_Constant
		+ light.SpotLight_Linear * len
		+ light.SpotLight_Quad * len * len);

	// Calc basic ambient factor.
	ambient = light.Color * light.Ambient * material.Ambient * att;

	// Calc light angle factor.
	float theta = dot(lightDir, -l);
	float epsilon = light.SpotLight_CutOff - light.SpotLight_OuterCutOff;
	float intensity = saturate((theta - light.SpotLight_OuterCutOff) / epsilon);

	const float diffuseFactor = angleFactor * intensity * att;
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		// Check shadow caster.
		RayDesc toLight = (RayDesc)0;
		toLight.Origin = posW;
		toLight.Direction = l;
		toLight.TMin = 0.01f;
		toLight.TMax = len;

		Payload shadowPayload = (Payload)0;
		Payload_Type(shadowPayload, PayloadType_ShadowCast);
		TraceRaySimply(shadowPayload, toLight);

		if (!Payload_bShadowHit(shadowPayload))
		{
			float3 v = reflect(direction, normal);
			float specFactor = pow(max(dot(v, rayDir), 0.0f), max(material.SpecExp, 1.0f)) * att * intensity;

			diffuse = light.Color * diffuseFactor * light.Diffuse * material.Diffuse;
			specular = specFactor * light.Specular * material.Specular;
		}
	}

	// Make light color.
	return ambient + diffuse + specular;
}

inline RayFragment CalcFragment(uint primitiveId, float2 bary)
{
	// Get primitive triangle.
	Vertex triangles[3];

	uint primitiveID = PrimitiveIndex() * 3;
	uint triangleIndex[3] = { primitiveID + 0, primitiveID + 1, primitiveID + 2 };

	[unroll]
	for (uint i = 0; i < 3; ++i)
	{
		triangleIndex[i] = cIndexBuffer[triangleIndex[i]];
		triangles[i] = cVertexBuffer[triangleIndex[i]];
	}

	// Interpolate fragment attributes.
	RayFragment frag;
	frag.PosW = WorldRayOrigin() + RayTCurrent() * WorldRayDirection();
	frag.Tex = HitAttribute(triangles[0].Tex, triangles[1].Tex, triangles[2].Tex, bary);
	frag.NormalW = normalize(mul(HitAttribute(triangles[0].Normal, triangles[1].Normal, triangles[2].Normal, bary), (float3x3)cTransform.WorldInvTranspose));
	//frag.TangentW = normalize(mul(triangles[0].TangentL * bary.x + triangles[1].TangentL * bary.y + triangles[2].TangentL * bary.z, (float3x3)cTransform.World));

	return frag;
}

inline void ClosestHit_OpaqueHit(inout Payload payload, RayFragment frag)
{
	uint numLights;
	uint lightStride;
	gLights.GetDimensions(numLights, lightStride);

	float3 lights = 0;
	for (uint i = 0; i < numLights; ++i)
	{
		GeneralLight light = gLights[i];
		switch (light.Type)
		{
		case LightType_Directional:
			lights += ComputeDirectionalLight(cMaterial, light, frag.NormalW, -WorldRayDirection(), frag.PosW);
			break;
		case LightType_Point:
			lights += ComputePointLight(cMaterial, light, frag.NormalW, -WorldRayDirection(), frag.PosW);
			break;
		case LightType_Spot:
			lights += ComputeSpotLight(cMaterial, light, frag.NormalW, -WorldRayDirection(), frag.PosW);
			break;
		}
	}

	float4 diffuse = lerp((float4)1.0f, cDiffuseMap.SampleLevel(gSampler, frag.Tex, 0), cMaterial.bDiffuseMap);
	diffuse.xyz *= lights;

	payload.Color = diffuse;
	payload.Pos = frag.PosW;
	payload.Normal = frag.NormalW;
}

inline void ClosestHit_ShadowCast(inout Payload payload, RayFragment frag)
{
	Payload_bShadowHit(payload, true);
}

[shader("closesthit")]
void ClosestHit(inout Payload payload : SV_Payload, BuiltInAttr attr)
{
	// Interpolate fragment attributes.
	RayFragment frag = CalcFragment(PrimitiveIndex(), attr.Value);
	int type = Payload_Type(payload);

	if (type == PayloadType_OpaqueHit)
	{
		ClosestHit_OpaqueHit(payload, frag);
	}
	else if (type == PayloadType_ShadowCast)
	{
		ClosestHit_ShadowCast(payload, frag);
	}
}