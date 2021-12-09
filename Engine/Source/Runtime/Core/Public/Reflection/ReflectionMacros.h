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

#define GENERATED_BODY(Class, ...)															\
	friend class Type;																		\
																							\
public:																						\
	using Super = 																			\
		typename ReflectionMacros::SuperClassTypeDeclare									\
		<Class __VA_OPT__(<) __VA_ARGS__ __VA_OPT__(>)>::Type;								\
	using This = Class __VA_OPT__(<) __VA_ARGS__ __VA_OPT__(>);								\
																							\
	static Type* StaticClass();																\
	inline static constexpr std::wstring_view FriendlyName = L ## #Class;					\
																							\
private:																					\
	static Type* StaticRegisterClass;														\
																							\
public:																						\
	Type* GetType() const																	\
	{																						\
		return StaticClass();																\
	}																						\
																							\
private:																					\
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
	template<size_t _Line>																	\
	static consteval size_t REFLECTION_PropertyChain()										\
	{																						\
		return REFLECTION_PropertyChain<_Line - 1>();										\
	}																						\
																							\
	template<>																				\
	static consteval size_t REFLECTION_PropertyChain<__LINE__>()							\
	{																						\
		return -1;																			\
	}																						\
																							\
	template<size_t>																		\
	static void REFLECTION_GetFunctionPointer(void*);										\
	template<size_t>																		\
	static consteval void REFLECTION_GetFunctionName(void*);								\
	template<size_t>																		\
	static void REFLECTION_GetPropertyPointer(void*);

#define GENERATE_BODY(Class, ...)															\
Type* Class::StaticClass()																	\
{																							\
	static Type MyClassType(Type::TypeGenerator<This>(FriendlyName, L ## #Class));			\
	return &MyClassType;																	\
}																							\
																							\
Type* Class::StaticRegisterClass = Class::StaticClass();

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

#define SPROPERTY(PropertyName, ...)														\
	template<>																				\
	static consteval size_t REFLECTION_PropertyChain<__LINE__>()							\
	{																						\
		return REFLECTION_PropertyChain<__LINE__ - 1>() + 1;								\
	}																						\
																							\
	template<size_t N> requires (N == REFLECTION_PropertyChain<__LINE__>())					\
	static auto REFLECTION_GetPropertyPointer(int)											\
	{																						\
		using PT = decltype(This::PropertyName);											\
		static Property::PropertyGenerator Generator =										\
		{																					\
			.Name = L ## #PropertyName,														\
			.MemberType = Type::GetStaticClass<												\
			std::remove_const_t<std::remove_pointer_t<decltype(This::PropertyName)>>>(),	\
			.Setter = +[](SObject* _this, const void* _value)								\
	{ dynamic_cast<This*>(_this)->PropertyName = *reinterpret_cast<const PT*>(_value); },	\
			.Getter = +[](const SObject* _this) -> const void*								\
			{ return &dynamic_cast<const This*>(_this)->PropertyName; },					\
			.bIsPointer = std::is_pointer_v<PT>,											\
			.bIsConst = std::is_const_v<PT>,												\
			.bIsStatic = Property::__Internal_IsStaticMember(&This::PropertyName),			\
		};																					\
		return &Generator;																	\
	}