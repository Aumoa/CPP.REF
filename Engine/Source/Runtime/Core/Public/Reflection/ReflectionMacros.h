// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Type.h"

namespace ReflectionMacros
{
	template<class T>
	struct SuperClassTypeDeclare
	{
	private:
		template<class U> requires requires { typename U::This; }
		static auto Impl(int32) -> decltype(std::declval<typename U::This>());

		template<class U> requires (!requires { typename U::This; })
		static auto Impl(int16) -> void;

	public:
		using Type = std::remove_reference_t<decltype(Impl<T>(0))>;
	};
}

#define GENERATED_BODY(Class)																\
public:																						\
	using Super = typename ReflectionMacros::SuperClassTypeDeclare<Class>::Type;			\
	using This = Class;																		\
																							\
	static const Type& StaticClass()														\
	{																						\
		static Type MyClassType(Type::TypeGenerator<Class>(L ## #Class));					\
		return MyClassType;																	\
	}																						\
																							\
	virtual const Type& GetType() const														\
	{																						\
		return StaticClass();																\
	}