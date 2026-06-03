// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance.h"
#include "SampleGame.h"
#include "SampleGameInstance.gen.h"

namespace Ayla
{
	class StaticMesh;
}

namespace SampleGame
{
	ACLASS()
	class SampleGameInstance : public GameInstance
	{
		GENERATED_BODY()

	private:
		SharedPtr<StaticMesh> m_BoxMesh;

	public:
		ACONSTRUCTOR()
		SampleGameInstance();
		virtual ~SampleGameInstance() noexcept override;

		virtual SharedPtr<Scene> GetEntryScene_Implementation() override;
	};
}