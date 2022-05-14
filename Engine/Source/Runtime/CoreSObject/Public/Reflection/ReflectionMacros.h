// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <tuple>
#include <iostream>
#include "Misc/RecursiveMacroHelper.h"

namespace libty::inline Core
{
	class SObject;
	class SType;

	namespace Reflection
	{
		class SAssembly;

		template<class T>
		struct SuperClassTypeDeclare
		{
		private:
			template<class U> requires requires { typename U::This; }
			static auto Impl(int) -> decltype(std::declval<typename U::This>());

			template<class U> requires (!requires { typename U::This; })
			static auto Impl(short) -> void;

		public:
			using Type = std::remove_reference_t<decltype(Impl<T>(0))>;
		};

		template<class... TInterfaces>
		class InterfaceCollector
		{
		public:
			template<class T>
			struct InterfaceCollection_t
			{
				template<class U> requires requires { std::declval<typename U::InterfaceCollection>(); }
				static auto Declare(int) -> typename U::InterfaceCollection;

				template<class U>
				static auto Declare(short) -> std::tuple<>;

				using Type = decltype(Declare<T>(0));
			};

		public:
			using InterfaceCollection = decltype(std::tuple_cat(std::declval<std::tuple<TInterfaces...>>(), std::declval<typename InterfaceCollection_t<TInterfaces>::Type>()...));
		};
	}
}

#define REFLECTION_FOREACH_CLASS_ATTRIBUTE_NAME(X) ::Attributes::SClassAttribute ## X
#define REFLECTION_FOREACH_FIELD_ATTRIBUTE_NAME(X) ::Attributes::SFieldAttribute ## X
#define REFLECTION_FOREACH_METHOD_ATTRIBUTE_NAME(X) ::Attributes::SMethodAttribute ## X

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
	friend struct ::libty::Core::Reflection::TypeInfoMetadataGenerator;						\
	friend struct ::libty::Core::Reflection::MethodInfoMetadataGenerator;					\
	static consteval ::libty::int32 __INTERNAL_AccessModifierChecker();						\
																							\
public:																						\
	using Super = typename ::libty::Core::Reflection::SuperClassTypeDeclare<Class>::Type;	\
	using This = Class;																		\
																							\
	inline static constexpr ::libty::String FriendlyName = TEXT(#Class);					\
																							\
private:																					\
	static ::libty::Core::SType StaticClass;												\
	static inline std::tuple AttributeCollection = std::make_tuple(__VA_OPT__(				\
		MACRO_RECURSIVE_FOR_EACH_DOT(REFLECTION_FOREACH_CLASS_ATTRIBUTE_NAME, __VA_ARGS__)	\
	));																						\
																							\
public:																						\
	static inline ::libty::Core::SType* TypeId = &StaticClass;								\
	::libty::Core::SType* GetType() const													\
	{																						\
		return TypeId;																		\
	}																						\
																							\
private:																					\
	GENERATED_BODY_DECLARE_REFLECTION_CHAINS()												\
																							\
private:

#define GENERATED_INTERFACE_BODY(Class, ...)												\
	friend struct ::libty::Core::Reflection::TypeInfoMetadataGenerator;						\
	friend struct ::libty::Core::Reflection::MethodInfoMetadataGenerator;					\
	static consteval ::libty::int32 __INTERNAL_AccessModifierChecker();						\
																							\
public:																						\
	inline static constexpr ::libty::String FriendlyName = TEXT(#Class);					\
																							\
private:																					\
	static ::libty::Core::SType StaticClass;												\
	static inline std::tuple AttributeCollection = std::make_tuple(__VA_OPT__(				\
		MACRO_RECURSIVE_FOR_EACH_DOT(REFLECTION_FOREACH_CLASS_ATTRIBUTE_NAME, __VA_ARGS__)	\
	));																						\
																							\
public:																						\
	static inline ::libty::Core::SType* TypeId = &StaticClass;								\
																							\
private:																					\
	GENERATED_BODY_DECLARE_REFLECTION_CHAINS()												\
																							\
private:

#define GENERATE_BODY(Class, ...)															\
namespace libty::Generated::Assemblies														\
{																							\
	extern ::libty::Core::Reflection::SAssembly SE_ASSEMBLY_INFO;							\
}																							\
																							\
namespace libty::Generated::Class															\
{																							\
	inline constexpr bool __pred__															\
		= ::libty::Core::Reflection::IInternalAccessModifierIsPublic<::Class>;				\
}																							\
																							\
::libty::Core::SType __VA_ARGS__ (::Class::StaticClass) =									\
	::libty::Core::SType(::libty::Core::Reflection											\
	::TypeInfoMetadataGenerator::GenerateManaged<(char)FriendlyName[0], ::Class>(			\
		FriendlyName,																		\
		TEXT(#Class),																		\
		&::libty::Generated::Assemblies::SE_ASSEMBLY_INFO,									\
		AttributeCollection																	\
	)																						\
);

#define SFUNCTION(FunctionName, ...)														\
	template<size_t _Line> requires (_Line == __LINE__)										\
	static consteval size_t REFLECTION_FunctionChain()										\
	{																						\
		return REFLECTION_FunctionChain<__LINE__ - 1>() + 1;								\
	}																						\
																							\
	template<size_t N> requires (N == REFLECTION_FunctionChain<__LINE__>())					\
	static auto REFLECTION_GetFunctionPointer(int)											\
	{																						\
		static std::tuple AttributeCollection = std::make_tuple(__VA_OPT__(					\
			MACRO_RECURSIVE_FOR_EACH_DOT(													\
				REFLECTION_FOREACH_METHOD_ATTRIBUTE_NAME,									\
				__VA_ARGS__																	\
			)																				\
		));																					\
		static ::libty::Core::Reflection::SMethodInfo MethodInfo = ::libty::Core::Reflection\
			::MethodInfoMetadataGenerator::Generate(										\
				&This::FunctionName,														\
				TEXT(#FunctionName),														\
				AttributeCollection															\
			);																				\
		return &MethodInfo;																	\
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
		static std::tuple AttributeCollection = std::make_tuple(__VA_OPT__(					\
			MACRO_RECURSIVE_FOR_EACH_DOT(													\
				REFLECTION_FOREACH_FIELD_ATTRIBUTE_NAME,									\
				__VA_ARGS__																	\
			)																				\
		));																					\
		static ::libty::Core::Reflection::SFieldInfo FieldInfo = ::libty::Core::Reflection	\
			::FieldInfoMetadataGenerator													\
			(																				\
				&This::PropertyName,														\
				TEXT(#PropertyName),														\
				AttributeCollection															\
			);																				\
		return &FieldInfo;																	\
	}

#define SVIRTUAL_IMPLEMENTS_NODE(X) virtual public X

#define implements(...) public ::libty::Core::Reflection::InterfaceCollector<__VA_ARGS__>, MACRO_RECURSIVE_FOR_EACH_DOT(SVIRTUAL_IMPLEMENTS_NODE, __VA_ARGS__)