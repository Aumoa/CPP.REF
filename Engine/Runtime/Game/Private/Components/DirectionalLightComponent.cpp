// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/DirectionalLightComponent.h"

#include "SceneRendering/LightSceneProxy.h"
#include "SceneRendering/MinimalViewInfo.h"
#include "RHI/RHICommon.h"
#include "RHI/IRHIResource.h"

class DirectionalLightSceneProxy : public LightSceneProxy
{
public:
	using Super = LightSceneProxy;
	using This = DirectionalLightSceneProxy;

private:
	GDirectionalLightComponent* myComponent;

public:
	DirectionalLightSceneProxy(GDirectionalLightComponent* inComponent) : Super(inComponent)
		, myComponent(inComponent)
	{

	}

	void UpdateBatchBuffer() override
	{
		Super::UpdateBatchBuffer();

		auto lightBatch = GetLightBatch();
		RHILight& light = *(RHILight*)lightBatch->GetLightBuffer()->GetMappingAddress();

		light.Type = RHILight::LIGHT_TYPE_DIRECTIONAL;
		light.Ambiguous_01 = myComponent->ComponentRotation.RotateVector(Vector3::Forward).Cast<Vector4>();
	}
};

GDirectionalLightComponent::GDirectionalLightComponent() : Super()
{

}

GDirectionalLightComponent::~GDirectionalLightComponent()
{

}

TRefPtr<LightSceneProxy> GDirectionalLightComponent::CreateSceneProxy()
{
	return NewObject<DirectionalLightSceneProxy>(this);
}

void GDirectionalLightComponent::CalcLightView(MinimalViewInfo& outViewInfo) const
{
	outViewInfo.FOV = 0;
	outViewInfo.AspectRatio = 1.0f;
	outViewInfo.Location = ComponentLocation;
	outViewInfo.Rotation = ComponentRotation;
}