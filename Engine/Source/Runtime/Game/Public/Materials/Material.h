// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MaterialInterface.h"
#include "Material.generated.h"

SCLASS()
class GAME_API Material : public MaterialInterface
{
	GENERATED_BODY()

public:
	Material();
	virtual ~Material() noexcept override;
};