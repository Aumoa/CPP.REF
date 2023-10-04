// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineSubsystem.h"
#include "GameEngine.h"
#include "Assets/StreamableRenderAsset.h"
#include "StreamableAssetManager.generated.h"

ACLASS()
class ENGINE_API AStreamableAssetManager : public AEngineSubsystem
{
	GENERATED_BODY()

private:
	std::map<String, WeakPtr<AStreamableRenderAsset>> StreamableAssets;

public:
	AStreamableAssetManager();
	virtual ~AStreamableAssetManager() noexcept override;

public:
	RefPtr<AStreamableRenderAsset> LoadObject(String InPath);
};

inline RefPtr<AStreamableRenderAsset> LoadObject(String InPath)
{
	return GEngine->GetEngineSubsystems().GetSubsystem<AStreamableAssetManager>()->LoadObject(InPath);
}