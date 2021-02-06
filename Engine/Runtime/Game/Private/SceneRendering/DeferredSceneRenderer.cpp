// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/DeferredSceneRenderer.h"

#include "Engine.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/SceneVisibility.h"
#include "SceneRendering/MeshBatch.h"
#include "SceneRendering/PrimitiveSceneProxy.h"
#include "SceneRendering/ShaderCameraConstant.h"
#include "RHI/RHICommon.h"
#include "RHI/RHIShaderLibrary.h"
#include "RHI/IRHICommandList.h"
#include "RHI/IRHIDeviceBundle.h"
#include "Logging/LogMacros.h"

using namespace std;

DeferredSceneRenderer::DeferredSceneRenderer(Scene* inScene) : Super(inScene)
	, shaderLibrary(nullptr)
{
	shaderLibrary = GEngine.DeviceBundle->GetShaderLibrary();
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
	IRHIShader* shader = shaderLibrary->GetShader(RHIShaderLibrary::GeometryShader);
	commandList->SetShader(shader);
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