// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Actor.h"

namespace libty::inline Game
{
	class APawn;

	class GAME_API AController : public AActor
	{
		GENERATED_BODY(AController);

	private:
		SPROPERTY(PossessedPawn)
		APawn* PossessedPawn = nullptr;

	public:
		AController();

		void Possess(APawn* InNewPawn);
		void UnPossess();

		APawn* GetPawn() const;
		template<std::derived_from<APawn> T> T* GetPawn() const { return Cast<T>(GetPawn()); }

	protected:
		virtual void OnPossess(APawn* pawn) {}
		virtual void OnUnPossess() {}
	};
}