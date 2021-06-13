﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Actor.h"

class GInputComponent;
class AController;

template<class T>
concept TIsController = requires(AController* lh, T* rh)
{
	{ lh = rh };
};

class GAME_API APawn : public AActor
{
public:
	using Super = AActor;
	using This = APawn;

private:
	AController* myController;

public:
	APawn();
	~APawn() override;

	virtual void SetupPlayerInputComponent(GInputComponent* inPlayerInput);
	virtual void PossessedBy(AController* inNewController);
	virtual void UnPossessed();

	template<TIsController T = AController>
	T* GetController() const;
	AController* GetController() const;
};

#include "Pawn.inl"