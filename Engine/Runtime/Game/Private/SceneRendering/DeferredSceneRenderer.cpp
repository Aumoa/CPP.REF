// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/DeferredSceneRenderer.h"

#include "Engine.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/SceneVisibility.h"
#include "SceneRendering/MeshBatch.h"
#include "SceneRendering/PrimitiveSceneProxy.h"
#include "Shaders/ShaderCameraConstant.h"
#include "RHI/RHICommon.h"
#include "RHI/RHIShaderLibrary.h"
#include "RHI/IRHICommandList.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHI/IRHIShader.h"
#include "Logging/LogMacros.h"

using namespace std;

bool DeferredSceneRenderer::bShaderCompiled = false;
IRHIShader* DeferredSceneRenderer::pickShader = nullptr;

DeferredSceneRenderer::DeferredSceneRenderer(Scene* inScene) : Super(inScene)
{
	ShaderInitialize();
}

DeferredSceneRenderer::~DeferredSceneRenderer()
{

}

void DeferredSceneRenderer::RenderScene(IRHICommandList* immediateCommandList)
{
	SetShader(immediateCommandList);

	const auto& visibilities = GetSceneVisibilities();

	for (size_t i = 0; i < visibilities.size(); ++i)
	{
		const SceneVisibility& visibility = visibilities[i];
		const vector<bool> primitiveVisibility = visibility.PrimitiveVisibility;
		RenderSceneInternal(immediateCommandList, primitiveVisibility);
	}
}

void DeferredSceneRenderer::SetShader(IRHICommandList* commandList)
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

using BYTE = unsigned char;

#include "Shaders/Compiled/VertexShader.hlsl.h"
#include "Shaders/Compiled/PixelShader.hlsl.h"

void DeferredSceneRenderer::ShaderInitialize()
{
	if (!bShaderCompiled)
	{
		RHIShaderLibrary* shaderLibrary = GEngine.DeviceBundle->GetShaderLibrary();

		RHIShaderDescription shaderDesc;
		shaderDesc.ShaderName = "Engine/SceneRendererShader";
		shaderDesc.VS = RHIShaderBytecode(pVertexShader);
		shaderDesc.PS = RHIShaderBytecode(pPixelShader);
		shaderDesc.RTVFormats = { ERHITextureFormat::B8G8R8A8_UNORM, ERHITextureFormat::R16G16B16A16_FLOAT };
		shaderDesc.DSVFormat = ERHITextureFormat::D24_UNORM_S8_UINT;
		TRefPtr<IRHIShader> shader = GEngine.DeviceBundle->CreateShader(shaderDesc);
		pickShader = shader.Get();

		shaderLibrary->AddShader(shader);
		bShaderCompiled = true;
	}
}

void DeferredSceneRenderer::RenderSceneInternal(IRHICommandList* commandList, const vector<bool>& primitiveVisibility)
{
	span<PrimitiveSceneProxy* const> primitiveSceneProxies = TargetScene->PrimitiveSceneProxies;
	size_t numSceneProxies = primitiveSceneProxies.size();

	CameraConstantIterator cbvIterator = TargetScene->ShaderCameraConstants->GetBufferIterator();

	for (size_t i = 0; i < numSceneProxies; ++i)
	{
		if (primitiveVisibility[i])
		{
			uint64 cbv = cbvIterator.Current();

			PrimitiveSceneProxy* scene = primitiveSceneProxies[i];
			MeshBatch* batch = scene->GetMeshBatch();
			const RHIMeshDrawCommand* drawCommand = batch->GetDrawCommand();
			commandList->SetGraphicsRootConstantBufferView(0, cbv);
			commandList->DrawMesh(*drawCommand);

			cbvIterator.MoveNext();
		}
	}
}