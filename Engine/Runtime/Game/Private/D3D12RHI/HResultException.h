// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"

class HResultException : public Exception
{
public:
	using Super = Exception;
	using This = HResultException;

public:
	HResultException(HRESULT hr);
	~HResultException() override;

private:
	static TRefPtr<String> GetHResultString(HRESULT hr);
};