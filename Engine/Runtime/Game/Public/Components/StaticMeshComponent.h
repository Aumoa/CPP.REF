// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "MeshComponent.h"

namespace SC::Runtime::Game::Components
{
	class GAME_API StaticMeshComponent : public MeshComponent
	{
	public:
		using Super = MeshComponent;
		using This = StaticMeshComponent;

	public:
		StaticMeshComponent();
		~StaticMeshComponent() override;
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::Components::StaticMeshComponent;
#endif