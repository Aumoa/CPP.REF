// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class GAME_API DirectXDeviceBundle : virtual public Object
{
public:
	using Super = Object;

public:
	DirectXDeviceBundle();
	~DirectXDeviceBundle() override;

	void Initialize();
};