// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"

class AChessBoardProxy;

class AChessBoard : public AActor
{
public:
	using Super = AActor;

private:
	Degrees _rotation;

public:
	AChessBoard();

	virtual void TickActor(std::chrono::duration<float> elapsedTime, ActorTickFunction* tickFunction) override;

	AChessBoardProxy* CreateProxy();
};