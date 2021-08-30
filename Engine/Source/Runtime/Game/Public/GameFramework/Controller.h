// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor.h"

class APawn;

/// <summary>
/// Represents controller that, possess to pawn and control the pawn.
/// </summary>
class GAME_API AController : public AActor
{
	CLASS_BODY(AController)

private:
	APawn* _possessedPawn = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="AController"/> instance.
	/// </summary>
	/// <param name="name"> The controller name. </param>
	AController();

	/// <summary>
	/// Possess to target pawn.
	/// </summary>
	void Possess(APawn* pawn);

	/// <summary>
	/// Unpossess from current target.
	/// </summary>
	void UnPossess();

	APawn* GetPawn() const;
	template<std::derived_from<APawn> T>
	T* GetPawn() const
	{
		return dynamic_cast<T*>(GetPawn());
	}

protected:
	virtual void OnPossess(APawn* pawn) {}
	virtual void OnUnPossess() {}
};