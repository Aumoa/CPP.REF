// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Materials/MaterialInterface.h"

class MaterialInstance : public MaterialInterface
{
public:
	using Super = MaterialInterface;
	using This = MaterialInstance;

public:
	MaterialInstance();
	~MaterialInstance() override;
};