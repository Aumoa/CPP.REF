// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Actor.h"

namespace SC::Runtime::Game::Components
{
	class StaticMeshComponent;
}

namespace SC::Runtime::Game::Framework
{
	class GAME_API AStaticMeshActor : public AActor
	{
	public:
		using Super = AActor;
		using This = AStaticMeshActor;

	private:
		Components::StaticMeshComponent* staticMeshComponent;

	public:
		AStaticMeshActor();
		~AStaticMeshActor();
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::Framework::AStaticMeshActor;
#endif