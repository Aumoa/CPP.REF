// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Rendering/RenderScene.h"
#include "Rendering/PrimitiveSceneInfo.h"
#include "RHI/RHIRenderThread.h"

RenderScene::RenderScene(World* OwnedWorld)
{
}

void RenderScene::AddPrimitive(PrimitiveSceneInfo* InPrimitive)
{
	check(InPrimitive->IsRegistered());

	RHIRenderThread::Get()->EnqueueWork([this, InPrimitive]()
	{
		// One more checks for duplicate register.
		check(InPrimitive->IsRegistered());

		size_t PrimitiveId;
		if (PendingPrimitiveIds.empty())
		{
			PrimitiveId = Primitives.size();
			Primitives.emplace_back();
		}
		else
		{
			PrimitiveId = PendingPrimitiveIds.back();
			PendingPrimitiveIds.erase(PendingPrimitiveIds.end() - 1);
		}

		Primitives[PrimitiveId] = InPrimitive;
		InPrimitive->PrimitiveId = PrimitiveId;
	});
}

void RenderScene::RemovePrimitive(size_t PrimitiveId)
{
	RHIRenderThread::Get()->EnqueueWork([this, PrimitiveId]()
	{
		PrimitiveSceneInfo*& PrimitiveInfo = Primitives[PrimitiveId];
		check(PrimitiveInfo);

		if (PrimitiveInfo)
		{
			size_t PrimitiveId = PrimitiveInfo->PrimitiveId;
			PrimitiveInfo->PrimitiveId = -1;
			PrimitiveInfo = nullptr;

			PendingPrimitiveIds.emplace_back(PrimitiveId);
		}
	});
}