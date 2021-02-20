// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct RHIMeshDrawCommand;

class GAME_API MeshBatch : virtual public Object
{
public:
	using Super = Object;
	using This = MeshBatch;

public:
	MeshBatch();
	~MeshBatch();

	virtual const RHIMeshDrawCommand* GetDrawCommand() const = 0;
};