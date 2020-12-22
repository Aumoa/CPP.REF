// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game
{
	class GAME_API GameInstance : virtual public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = GameInstance;

	private:
		static Core::TRefPtr<Core::String> defaultAppName;

	public:
		GameInstance();
		~GameInstance() override;

		Core::TRefPtr<Core::String> ToString() const override;
	};
}