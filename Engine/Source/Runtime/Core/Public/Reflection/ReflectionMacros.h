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
	static Type* StaticClass()																\
	{																						\
		static Type MyClassType(Type::TypeGenerator<Class>(L ## #Class));					\
		return &MyClassType;																\
	}																						\
																							\
	virtual Type* GetType() const															\
	{																						\
		return StaticClass();																\
	}																						\
																							\
public:																						\
	template<size_t _Line>																	\
	static consteval size_t REFLECTION_FunctionChain()										\
	{																						\
		return REFLECTION_FunctionChain<_Line - 1>();										\
	}																						\
																							\
	template<>																				\
	static consteval size_t REFLECTION_FunctionChain<__LINE__>()							\
	{																						\
		return -1;																			\
	}																						\
																							\
	template<size_t>																		\
	static void REFLECTION_GetFunctionPointer(void*);										\
	template<size_t>																		\
	static consteval void REFLECTION_GetFunctionName(void*);

#define SFUNCTION(FunctionName, ...)														\
	template<>																				\
	static consteval size_t REFLECTION_FunctionChain<__LINE__>()							\
	{																						\
		return REFLECTION_FunctionChain<__LINE__ - 1>() + 1;								\
	}																						\
																							\
	template<size_t N> requires (N == REFLECTION_FunctionChain<__LINE__>())					\
	static auto REFLECTION_GetFunctionPointer(int)											\
	{																						\
		return &This::FunctionName;															\
	}																						\
																							\
	template<size_t N> requires (N == REFLECTION_FunctionChain<__LINE__>())					\
	static consteval auto REFLECTION_GetFunctionName(int)									\
	{																						\
		return L ## #FunctionName;															\
	}