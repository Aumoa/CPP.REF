// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderer.h"

#include "SceneRendering/SceneVisibility.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/PrimitiveSceneProxy.h"
#include "SceneRendering/MeshBatch.h"
#include "Framework/PlayerController.h"
#include "Components/PlayerCameraManager.h"
#include "Logging/LogMacros.h"
#include "RHI/RHIMeshDrawCommand.h"
#include "RHI/IRHICommandList.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHI/RHIShaderLibrary.h"
#include "RHI/RHIShaderDescription.h"
#include "RHI/IRHIShader.h"
#include "Engine.h"

using namespace std;

bool SceneRenderer::bShaderCompiled = false;
IRHIShader* SceneRenderer::pickShader = nullptr;

SceneRenderer::SceneRenderer(Scene* scene) : Super()
	, renderScene(scene)
{
	ShaderInitialize();
}

SceneRenderer::~SceneRenderer()
{

}

void SceneRenderer::CalcVisibility(MinimalViewInfo& inView)
{
	visibilities.emplace_back(renderScene, inView);
	visibilities.back().CalcVisibility();
}

void SceneRenderer::RenderScene(IRHICommandList* immediateCommandList)
{
	SetShader(immediateCommandList);

	for (size_t i = 0; i < visibilities.size(); ++i)
	{
		SceneVisibility& visibility = visibilities[i];
		const vector<bool> primitiveVisibility = visibility.PrimitiveVisibility;
		RenderSceneInternal(immediateCommandList, primitiveVisibility);
	}
}

void SceneRenderer::CalcLocalPlayerVisibility()
{
	APlayerController* localPlayer = renderScene->LocalPlayer;
	PlayerCameraManager* cameraManager = localPlayer->CameraManager;

	if (cameraManager == nullptr)
	{
		SE_LOG(LogRendering, Error, L"PlayerController have not a camera manager component. Abort.");
		return;
	}

	MinimalViewInfo viewInfo;
	cameraManager->CalcCameraView(viewInfo);

	CalcVisibility(viewInfo);
}

using BYTE = unsigned char;

#include "Shaders/Compiled/VertexShader.hlsl.h"
#include "Shaders/Compiled/PixelShader.hlsl.h"

void SceneRenderer::ShaderInitialize()
{
	if (!bShaderCompiled)
	{
		RHIShaderLibrary* shaderLibrary = GEngine.DeviceBundle->GetShaderLibrary();

		RHIShaderDescription shaderDesc;
		shaderDesc.ShaderName = "Engine/SceneRendererShader";
		shaderDesc.VS = RHIShaderBytecode(pVertexShader);
		shaderDesc.PS = RHIShaderBytecode(pPixelShader);
		TRefPtr<IRHIShader> shader = GEngine.DeviceBundle->CreateShader(shaderDesc);
		pickShader = shader.Get();

		shaderLibrary->AddShader(shader);
		bShaderCompiled = true;
	}
}

void SceneRenderer::SetShader(IRHICommandList* commandList)
{
	IRHIShader* shader = pickShader;

	if (!bShaderCompiled)
	{
		RHIShaderLibrary* shaderLibrary = GEngine.DeviceBundle->GetShaderLibrary();
		shader = shaderLibrary->GetShader(L"Engine/Default");
	}

#if defined(_DEBUG)
	if (shader == nullptr)
	{
		SE_LOG(LogRendering, Error, L"Cannot found desired shader and default shader. Abort.");
		return;
	}
#endif

	commandList->SetShader(shader);
}

void SceneRenderer::RenderSceneInternal(IRHICommandList* commandList, const vector<bool>& primitiveVisibility)
{
	span<PrimitiveSceneProxy* const> primitiveSceneProxies = renderScene->PrimitiveSceneProxies;
	size_t numSceneProxies = primitiveSceneProxies.size();

	for (size_t i = 0; i < numSceneProxies; ++i)
	{
		if (primitiveVisibility[i])
		{
			PrimitiveSceneProxy* scene = primitiveSceneProxies[i];
			MeshBatch* batch = scene->GetMeshBatch();
			const RHIMeshDrawCommand* drawCommand = batch->GetDrawCommand();
			commandList->DrawMesh(*drawCommand);
		}
	}
}