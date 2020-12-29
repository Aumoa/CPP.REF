// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "PrimitiveComponent.h"

namespace SC::Runtime::Game::Components
{
	class GAME_API MeshComponent : public PrimitiveComponent
	{
	public:
		using Super = PrimitiveComponent;
		using This = PrimitiveComponent;

	public:
		MeshComponent();
		~MeshComponent() override;
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::Components::MeshComponent;
#endif