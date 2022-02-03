// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor.h"

class AController;

/// <summary>
/// Represents pawn that controller can be possessed.
/// </summary>
class GAME_API APawn : public AActor
{
	GENERATED_BODY(APawn)

private:
	AController* _controller = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="APawn"/> instance.
	/// </summary>
	APawn();

	/// <summary>
	/// The function called when possessed by controller.
	/// </summary>
	virtual void PossessedBy(AController* controller);

	/// <summary>
	/// The function called when unpossessed.
	/// </summary>
	virtual void UnPossessed();

	/// <summary>
	/// Get current possessed controller.
	/// </summary>
	/// <returns></returns>
	template<std::derived_from<AController> T>
	T* GetController() const
	{
		return dynamic_cast<T>(_controller);
	}
};