// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Actor.h"

class APawn;

class GAME_API AController : public AActor
{
public:
	using Super = AActor;
	using This = AController;

private:
	TWeakPtr<APawn> possessedPawn;

public:
	AController();
	~AController() override;

	virtual void Possess(APawn* inPawn);
	virtual void UnPossess();

	APawn* GetPawn() const;
	template<class T> requires TIsBaseOf<T, APawn>
	T* GetPawn() const;
};

#include "Controller.inl"