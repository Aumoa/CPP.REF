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

	Scene* myScene = GetScene();
	std::vector<SceneVisibility*> visibilities = { myScene->GetLocalPlayerVisibility() };
	for (size_t i = 0; i < visibilities.size(); ++i)
	{
		SceneVisibility* visibility = visibilities[i];
		RenderSceneInternal(immediateCommandList, visibility);
	}
}

void DeferredSceneRenderer::SetShader(IRHICommandList* commandList)
{
	IRHIShader* shader = shaderLibrary->GetShader(RHIShaderLibrary::GeometryShader);
	commandList->SetShader(shader);
}

void DeferredSceneRenderer::RenderSceneInternal(IRHICommandList* commandList, SceneVisibility* inSceneVisibility)
{
	span<PrimitiveSceneProxy* const> primitiveSceneProxies = GetScene()->PrimitiveSceneProxies;
	const auto& primitiveVisibility = inSceneVisibility->PrimitiveVisibility;
	CameraConstantIterator cbvIterator = inSceneVisibility->ShaderCameraConstants->GetBufferIterator();

	for (size_t i = 0; i < primitiveSceneProxies.size(); ++i)
	{
		if (primitiveVisibility[i])
		{
			uint64 cbv = cbvIterator.Current();

			PrimitiveSceneProxy* scene = primitiveSceneProxies[i];
			MeshBatch* batch = scene->GetMeshBatch();
			const RHIMeshDrawCommand* drawCommand = batch->GetDrawCommand();
			uint32 const32Bit = drawCommand->MaterialIndex;
			commandList->SetGraphicsRootConstantBufferView(0, cbv);
			commandList->SetGraphicsRoot32BitConstants(1, &const32Bit, 1);
			commandList->DrawMesh(*drawCommand);

			cbvIterator.MoveNext();
		}
	}
}