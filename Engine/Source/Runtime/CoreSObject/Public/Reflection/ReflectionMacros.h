// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/RecursiveMacroHelper.h"
#include <tuple>

class SType;
class SObject;

namespace libty::Core::Reflection
{
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
	class InterfaceCollector : virtual public TInterfaces...
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

		virtual SType* GetType() const override
		{
			return (TInterfaces::SObject::TypeId, ...);
		}

	public:
		using InterfaceCollection = decltype(std::tuple_cat(std::declval<std::tuple<TInterfaces...>>(), std::declval<typename InterfaceCollection_t<TInterfaces>::Type>()...));
	};
}

#define REFLECTION_FOREACH_CLASS_ATTRIBUTE_NAME(X) SClassAttribute ## X
#define REFLECTION_FOREACH_FIELD_ATTRIBUTE_NAME(X) SFieldAttribute ## X
#define REFLECTION_FOREACH_METHOD_ATTRIBUTE_NAME(X) SMethodAttribute ## X

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
	friend struct libty::Core::Reflection::MethodInfoMetadataGenerator;						\
	static consteval int32 __INTERNAL_AccessModifierChecker();								\
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
		MACRO_RECURSIVE_FOR_EACH_DOT(REFLECTION_FOREACH_CLASS_ATTRIBUTE_NAME, __VA_ARGS__)	\
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
namespace libty::Generator::Class															\
{																							\
	inline constexpr bool __pred__															\
		= ::libty::Core::Reflection::IInternalAccessModifierIsPublic<::Class>;				\
}																							\
extern SAssembly SE_ASSEMBLY_INFO;															\
SType Class::StaticClass = SType(libty::Core::Reflection::TypeInfoMetadataGenerator			\
	::GenerateManaged<(char)Class::FriendlyName[0], Class>(									\
		Class::FriendlyName,																\
		L ## #Class,																		\
		&SE_ASSEMBLY_INFO,																	\
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
			MACRO_RECURSIVE_FOR_EACH_DOT(REFLECTION_FOREACH_METHOD_ATTRIBUTE_NAME, __VA_ARGS__)\
		));																					\
		static SMethodInfo MethodInfo = libty::Core::Reflection::MethodInfoMetadataGenerator\
			::Generate(																		\
				&This::FunctionName,														\
				#FunctionName,																\
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
			MACRO_RECURSIVE_FOR_EACH_DOT(REFLECTION_FOREACH_FIELD_ATTRIBUTE_NAME, __VA_ARGS__)\
		));																					\
		static SFieldInfo FieldInfo = libty::Core::Reflection::FieldInfoMetadataGenerator	\
		(																					\
			&This::PropertyName,															\
			#PropertyName,																	\
			AttributeCollection																\
		);																					\
		return &FieldInfo;																	\
	}

#define implements(...) public libty::Core::Reflection::InterfaceCollector<__VA_ARGS__>