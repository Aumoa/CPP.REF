// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/LightSceneProxy.h"

#include "Engine.h"
#include "GameplayStatics/ClassDependencyHelper.h"
#include "Components/LightComponent.h"
#include "DirectX/DirectXDynamicBufferAllocator.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "Logging/LogMacros.h"
#include "Shaders/ShaderTypes.h"

LightSceneProxy::LightSceneProxy(GLightComponent* inLightComponent, size_t inDesiredBuffSize) : Super()
	, Component(inLightComponent)
	, Mobility(Component->Mobility)
	, PrimitiveTransform(inLightComponent->ComponentTransform)
	, InstanceCBV(0)
{
	if (inDesiredBuffSize == 0)
	{
		inDesiredBuffSize = sizeof(ShaderTypes::GeneralLight);
	}

	Engine* engine = ClassDependencyHelper::GetEngine(inLightComponent);
	DirectXDeviceBundle* deviceBundle = engine->GetDeviceBundle();
	DirectXDynamicBufferAllocator* allocator = deviceBundle->GetOrCreateDynamicBufferAllocator(sizeof(ShaderTypes::GeneralLight));
	lightShaderBuf = NewObject<DirectXDynamicBuffer>(allocator);
	InstanceCBV = lightShaderBuf->GetGPUVirtualAddress();

	if (Mobility == EComponentMobility::Static)
	{
		auto* ptr = (ShaderTypes::GeneralLight*)lightShaderBuf->GetBufferPointer();
		ptr->Color = Component->LightColor.Cast<Vector3>();
		ptr->Ambient = Component->Ambient;
		ptr->Diffuse = Component->Diffuse;
		ptr->Specular = Component->Specular;
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