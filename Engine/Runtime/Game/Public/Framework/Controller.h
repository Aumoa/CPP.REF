// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Actor.h"

struct LogCategoryBase;
class APawn;

class GAME_API AController : public AActor
{
public:
	using Super = AActor;
	using This = AController;

private:
	static LogCategoryBase LogController;

	TWeakPtr<APawn> possessedPawn;

public:
	AController();
	~AController() override;

	void Possess(APawn* inPawn);
	void UnPossess();

	virtual void OnPossess(APawn* inPawn);
	virtual void OnUnPossess();

	APawn* GetPawn() const;
	template<class T> requires TIsBaseOf<T, APawn>
	T* GetPawn() const;
};

#include "Controller.inl"