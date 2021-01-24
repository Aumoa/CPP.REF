// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/DeferredSceneRenderer.h"

#include <span>
#include "Engine.h"
#include "RHI/IRHIDeferredCommandList.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHI/RHIShaderLibrary.h"
#include "RHI/RHIPrimitiveTopology.h"
#include "RHI/RHIViewport.h"
#include "SceneRendering/PrimitiveSceneProxy.h"
#include "SceneRendering/MeshBatch.h"

using namespace std;

DeferredSceneRenderer::DeferredSceneRenderer(IRHIDeviceBundle* deviceBundle) : Super(deviceBundle)
{

}

DeferredSceneRenderer::~DeferredSceneRenderer()
{

}

void DeferredSceneRenderer::BeginRender()
{
	Super::BeginRender();
}

void DeferredSceneRenderer::EndRender()
{
	Super::EndRender();
}

void DeferredSceneRenderer::PopulateRenderCommands()
{
	IRHIRenderTargetView* rtv = GetFinalColorRTV();
	CommandList->SetRenderTargets(1, &rtv);

	int32 sceneX, sceneY;
	GetSceneSize(sceneX, sceneY);

	CommandList->SetScissorRects(Rect(Vector2::Zero, Vector2((float)sceneX, (float)sceneY)));
	CommandList->SetViewports(RHIViewport(sceneX, sceneY));

	RHIShaderLibrary* shaderLibrary = GEngine.DeviceBundle->GetShaderLibrary();
	IRHIShader* test_shader = shaderLibrary->GetShader("Engine/Default");

	CommandList->SetShader(test_shader);
	CommandList->SetPrimitiveTopology(ERHIPrimitiveTopology::TRIANGLELIST);

	// Rendering primitives for each views.

	span<PrimitiveSceneProxy* const> primitives = GetPrimitivesView();
	for (auto& primitive : primitives)
	{
		MeshBatch* batch = primitive->GetMeshBatch();
		if (batch != nullptr)
		{
			const RHIMeshDrawCommand* meshDrawCommand = batch->GetDrawCommand();
			CommandList->DrawMesh(*meshDrawCommand);
		}
	}
}