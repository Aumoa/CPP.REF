// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include "Reflection/reflexpr.h"

namespace Ayla
{
	struct TransferFunction_reflexpr_field_null : public std::reflect::reflexpr_field<std::reflect::access_type::public_, int, 0>
	{
	};

	struct TransferFunction_reflexpr_method_null : public std::reflect::reflexpr_method<std::reflect::access_type::public_, void(*)(), static_cast<void(*)()>(0)>
	{
	};

	struct TransferFunction_reflexpr_constructor_null : public std::reflect::reflexpr_constructor<std::reflect::access_type::public_, void(*)(), static_cast<void(*)()>(0)>
	{
	};

	template<class T>
	concept TransferFunction = requires(T&& transfer)
	{
		{ transfer.template Transfer<TransferFunction_reflexpr_field_null>() } -> std::same_as<void>;
		{ transfer.template Transfer<TransferFunction_reflexpr_method_null>() } -> std::same_as<void>;
		{ transfer.template Transfer<TransferFunction_reflexpr_constructor_null>() } -> std::same_as<void>;
	};
}