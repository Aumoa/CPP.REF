// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MaterialInterface.generated.h"

SCLASS()
class GAME_API MaterialInterface : virtual public Object
{
	GENERATED_BODY()

protected:
	MaterialInterface();

public:
	virtual ~MaterialInterface() noexcept override;
};