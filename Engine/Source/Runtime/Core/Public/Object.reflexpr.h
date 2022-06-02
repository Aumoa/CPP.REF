// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "reflexpr.h"
#include "Object.h"

namespace libty::reflect
{
	struct Object_constructor_t
	{
		using arguments = void;
		using is_public_t = int;

		inline static decltype(auto) invoke(void* ptr) { return new(ptr) Object(); }
	};

	struct Object_Method_GetType_t
	{
		using is_template_t = int;
		using is_readonly_t = int;
		inline static decltype(auto) invoke(const void* ptr) { return ((Object*)ptr)->GetType(); }
	};

	struct Object_Method_ToString_t
	{
		using is_readonly_t = int;
		inline static decltype(auto) invoke(const void* ptr) { return ((Object*)ptr)->ToString(); }
	};
}

template<>
struct libty::reflect::reflexpr_t<Object>
{
	using is_class_t = int;
	using constructors_t = Variadic<libty::reflect::Object_constructor_t>;
	using methods_t = Variadic<libty::reflect::Object_Method_GetType_t>;
};