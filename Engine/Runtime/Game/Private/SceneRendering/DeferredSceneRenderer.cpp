// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/DeferredSceneRenderer.h"

#include "Engine.h"
#include "DirectX/DirectXCommon.h"
#include "SceneRendering/SceneVisibility.h"
#include "SceneRendering/PrimitiveSceneProxy.h"
#include "SceneRendering/ShaderCameraConstant.h"
#include "SceneRendering/Scene.h"
#include "Logging/LogMacros.h"
#include "Materials/MaterialInterface.h"

using namespace std;

DeferredSceneRenderer::DeferredSceneRenderer(Scene* inScene) : Super(inScene)
	, shaderLibrary(nullptr)
{
	//shaderLibrary = GEngine.DeviceBundle->GetShaderLibrary();
}

DeferredSceneRenderer::~DeferredSceneRenderer()
{

}

void DeferredSceneRenderer::RenderScene(ID3D12GraphicsCommandList4* immediateCommandList)
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

void DeferredSceneRenderer::SetShader(ID3D12GraphicsCommandList4* commandList)
{

}

void DeferredSceneRenderer::RenderSceneInternal(ID3D12GraphicsCommandList4* commandList, SceneVisibility* inSceneVisibility)
{
	//span<PrimitiveSceneProxy* const> primitiveSceneProxies = GetScene()->GetPrimitives();
	//const auto& primitiveVisibility = inSceneVisibility->PrimitiveVisibility;
	//CameraConstantIterator cbvIterator = inSceneVisibility->ShaderCameraConstants->GetBufferIterator();

	////IRHIMaterialBundle* materialBundle = GEngine.MaterialBundle;

	//for (size_t i = 0; i < primitiveSceneProxies.size(); ++i)
	//{
	//	if (primitiveVisibility[i])
	//	{
	//		uint64 cbv = cbvIterator.Current();

	//		PrimitiveSceneProxy* scene = primitiveSceneProxies[i];
	//		MeshBatch* batch = scene->GetMeshBatch();
	//		const RHIMeshDrawCommand* drawCommand = batch->GetDrawCommand();
	//		uint32 const32Bit = drawCommand->MaterialIndex;
	//		//MaterialInterface* material = materialBundle->FindMaterialFromIndex(drawCommand->MaterialIndex);

	//		commandList->SetGraphicsRootConstantBufferView(0, cbv);
	//		commandList->SetGraphicsRoot32BitConstants(1, &const32Bit, 1);
	//		//commandList->SetGraphicsRootShaderResourceView(2, material->SurfaceTextureSRV);
	//		commandList->DrawMesh(*drawCommand);

	//		cbvIterator.MoveNext();
	//	}
	//}
}