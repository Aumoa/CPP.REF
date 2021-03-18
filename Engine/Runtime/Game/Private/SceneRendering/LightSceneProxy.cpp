// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/LightSceneProxy.h"

#include "Engine.h"
#include "GameplayStatics/ClassDependencyHelper.h"
#include "Components/LightComponent.h"
#include "DirectX/DirectXDynamicBufferAllocator.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "Logging/LogMacros.h"

LightSceneProxy::LightSceneProxy(GLightComponent* inLightComponent) : Super()
	, Component(inLightComponent)
	, Mobility(Component->Mobility)
	, PrimitiveTransform(inLightComponent->ComponentTransform)
	, PrimitiveId(0)
{
	if (Mobility == EComponentMobility::Static)
	{
		PrimitiveLight.Color = Component->LightColor.Cast<Vector3>();
		PrimitiveLight.Ambient = Component->Ambient;
		PrimitiveLight.Diffuse = Component->Diffuse;
		PrimitiveLight.Specular = Component->Specular;
	}
}

LightSceneProxy::~LightSceneProxy()
{

}

void LightSceneProxy::Update()
{
	if (Mobility == EComponentMobility::Static)
	{
		SE_LOG(LogRendering, Error, L"You try to update light attribute, but owning component is static mobility. The action does not defined.");
		return;
	}

	auto* ptr = (ShaderTypes::GeneralLight*)lightShaderBuf->GetBufferPointer();
	ptr->Color = Component->LightColor.Cast<Vector3>();
	ptr->Ambient = Component->Ambient;
	ptr->Diffuse = Component->Diffuse;
	ptr->Specular = Component->Specular;
}

void LightSceneProxy::UpdateTransform()
{
	if (Mobility == EComponentMobility::Static)
	{
		SE_LOG(LogRendering, Error, L"You try to update transform, but owning component is static mobility. The action does not defined.");
		return;
	}

	PrimitiveTransform = Component->ComponentTransform;
}

void* LightSceneProxy::GetLightShaderBuffer() const
{
	return lightShaderBuf->GetBufferPointer();
}