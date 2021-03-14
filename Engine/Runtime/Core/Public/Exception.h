// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"

#include "TRefPtr.h"
#include "CoreString.h"

class CORE_API Exception
{
public:
	using This = Exception;

private:
	Exception* innerException;
	TRefPtr<String> message;
	TRefPtr<String> copy_name;

	Exception(const Exception&) = delete;

public:
	Exception();
	Exception(TRefPtr<String> message);
	Exception(TRefPtr<String> message, Exception* innerException);
	Exception(Exception&& other) noexcept;
	virtual ~Exception();

	vs_property_get(TRefPtr<String>, Name);
	vs_property_get(TRefPtr<String>, Message);
	vs_property_get(Exception*, InnerException);
};