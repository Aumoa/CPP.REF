// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"

#include "TRefPtr.h"
#include "CoreString.h"

namespace SC::Runtime::Core
{
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
		TRefPtr<String> Name_get() const;

		vs_property_get(TRefPtr<String>, Message);
		TRefPtr<String> Message_get() const;

		vs_property_get(Exception*, InnerException);
		Exception* InnerException_get() const;
	};
}