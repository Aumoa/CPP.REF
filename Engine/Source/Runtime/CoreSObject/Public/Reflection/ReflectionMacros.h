// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/RecursiveMacroHelper.h"

namespace libty::Core::Reflection
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

class SType;

#define REFLECTION_FOREACH_ATTRIBUTE_NAME(X) SAttribute ## X

#define GENERATED_BODY_DECLARE_REFLECTION_CHAINS() 											\
	template<size_t _Line>																	\
	static consteval size_t REFLECTION_FunctionChain()										\
	{																						\
		return REFLECTION_FunctionChain<_Line - 1>();										\
	}																						\
																							\
	template<size_t _Line> requires (_Line == __LINE__)										\
	static consteval size_t REFLECTION_FunctionChain()										\
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
	template<size_t _Line> requires (_Line == __LINE__)										\
	static consteval size_t REFLECTION_PropertyChain()										\
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

#define GENERATED_BODY(Class, ...)															\
	friend struct libty::Core::Reflection::TypeInfoMetadataGenerator;						\
																							\
public:																						\
	using Super = typename libty::Core::Reflection::SuperClassTypeDeclare<Class>::Type;		\
	using This = Class;																		\
																							\
	inline static constexpr std::wstring_view FriendlyName = L ## #Class;					\
																							\
private:																					\
	static SType StaticClass;																\
	static inline std::tuple AttributeCollection = std::make_tuple(__VA_OPT__(				\
		MACRO_RECURSIVE_FOR_EACH(REFLECTION_FOREACH_ATTRIBUTE_NAME, __VA_ARGS__)			\
	));																						\
																							\
public:																						\
	static inline SType* TypeId = &StaticClass;												\
	SType* GetType() const																	\
	{																						\
		return TypeId;																		\
	}																						\
																							\
private:																					\
	GENERATED_BODY_DECLARE_REFLECTION_CHAINS()												\
																							\
private:

#define GENERATE_BODY(Class, ...)															\
SType Class::StaticClass = SType(libty::Core::Reflection::TypeInfoMetadataGenerator			\
	::GenerateClass<Class>(Class::FriendlyName, #Class, AttributeCollection));

#define GENERATED_INTERFACE_BODY(Interface, ...)											\
	friend struct libty::Core::Reflection::TypeInfoMetadataGenerator;						\
																							\
	static inline std::tuple AttributeCollection = std::make_tuple(__VA_OPT__(				\
		MACRO_RECURSIVE_FOR_EACH(REFLECTION_FOREACH_ATTRIBUTE_NAME, __VA_ARGS__)			\
	));																						\
	static inline SType StaticClass = libty::Core::Reflection::TypeInfoMetadataGenerator	\
	::GenerateClass<Interface>(Interface::FriendlyName, #Interface, AttributeCollection);	\
																							\
	inline static constexpr std::wstring_view FriendlyName = L ## #Interface;				\
																							\
public:																						\
	static inline SType* TypeId = &StaticClass;												\
	SType* GetType() const																	\
	{																						\
		return TypeId;																		\
	}																						\
																							\
private:																					\
	GENERATED_BODY_DECLARE_REFLECTION_CHAINS()												\
																							\
public:

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
	template<size_t _Line> requires (_Line == __LINE__)										\
	static consteval size_t REFLECTION_PropertyChain()										\
	{																						\
		return REFLECTION_PropertyChain<__LINE__ - 1>() + 1;								\
	}																						\
																							\
	template<size_t N> requires (N == REFLECTION_PropertyChain<__LINE__>())					\
	static auto REFLECTION_GetPropertyPointer(int)											\
	{																						\
		static SFieldInfo FieldInfo = libty::Core::Reflection::FieldInfoMetadataGenerator	\
		(																					\
			&This::PropertyName,															\
			#PropertyName,																	\
			{}																				\
		);																					\
		return &FieldInfo;																	\
	}

#define implements virtual public 