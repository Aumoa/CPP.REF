// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <set>
#include <queue>

class World;
class PrimitiveSceneInfo;
class PrimitiveSceneProxy;

class GAME_API RenderScene
{
private:
	std::vector<PrimitiveSceneInfo*> Primitives;
	std::vector<size_t> PendingPrimitiveIds;

public:
	RenderScene(World* OwnedWorld);

	void DestroyScene(std::unique_ptr<RenderScene> OwningPtr);

	void AddPrimitive(PrimitiveSceneInfo* InPrimitive);
	void RemovePrimitive(size_t PrimitiveId);
	void UpdatePrimitive(size_t PrimitiveId, PrimitiveSceneProxy* SceneProxy);

	inline std::span<PrimitiveSceneInfo* const> GetPrimitives() const noexcept { return Primitives; }
};