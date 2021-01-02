// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class GAME_API Mesh : virtual public Object
{
public:
	using Super = Object;
	using This = Mesh;

public:
	Mesh();
	~Mesh() override;
};