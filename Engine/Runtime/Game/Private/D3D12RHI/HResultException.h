// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"

namespace SC::Runtime::Game::D3D12RHI
{
	class HResultException : public Core::Exception
	{
	public:
		using Super = Exception;
		using This = HResultException;

	public:
		HResultException(HRESULT hr);
		~HResultException() override;

	private:
		static Core::TRefPtr<Core::String> GetHResultString(HRESULT hr);
	};
}