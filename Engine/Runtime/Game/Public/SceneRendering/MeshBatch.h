// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct RHIMeshDrawCommand;

class GAME_API MeshBatch : virtual public Object
{
public:
	using Super = Object;
	using This = MeshBatch;

private:
	RHIMeshDrawCommand* drawCommand;

public:
	MeshBatch();
	~MeshBatch();

	RHIMeshDrawCommand* GetDrawCommand() const;
};