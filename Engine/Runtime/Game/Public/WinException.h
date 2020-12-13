// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game
{
	class GAME_API WinException : public Core::Exception
	{
	public:
		using Super = Core::Exception;
		using This = WinException;

	public:
		WinException();
		WinException(int32 lastError);
		~WinException() override;

	private:
		static Core::TRefPtr<Core::String> GetErrorMessage(int32 lastError);
	};
}