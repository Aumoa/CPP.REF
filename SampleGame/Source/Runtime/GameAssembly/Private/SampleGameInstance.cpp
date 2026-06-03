// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "SampleGameInstance.h"
#include "Engine.h"
#include "Graphics.h"
#include "SceneManagement/Scene.h"
#include "Actors/GameObject.h"
#include "Rendering/Camera.h"
#include "Rendering/StaticMesh.h"
#include "Rendering/StaticMeshRenderer.h"
#include "Components/FloatingMovementComponent.h"

namespace SampleGame
{
	SampleGameInstance::SampleGameInstance()
	{
	}

	SampleGameInstance::~SampleGameInstance() noexcept
	{
	}

	SharedPtr<Scene> SampleGameInstance::GetEntryScene_Implementation()
	{
		auto scene = New<Scene>();
		auto gameObject = scene->SpawnGameObject();
		gameObject->SetName(TEXT("Main Camera"));
		gameObject->AddComponent<Camera>();
		gameObject->AddComponent<FloatingMovementComponent>();
		gameObject = scene->SpawnGameObject();
		gameObject->SetName(TEXT("StaticMesh"));
		return scene;
	}
}
