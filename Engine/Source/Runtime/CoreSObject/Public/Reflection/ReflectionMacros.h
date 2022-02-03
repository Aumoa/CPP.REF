// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#define GENERATED_BODY(Class, ...)															\
	friend class Type;																		\
																							\
public:																						\
	using Super = typename ReflectionMacros::SuperClassTypeDeclare<Class>::Type;			\
	using This = Class;																		\
																							\
	static Type* StaticClass();																\
																							\
	inline static constexpr std::wstring_view FriendlyName = L ## #Class;					\
																							\
private:																					\
	inline static Type* StaticRegisterClass = StaticClass();								\
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
	static void REFLECTION_GetPropertyPointer(void*);										\
																							\
private:

#define GENERATED_INTERFACE_BODY(Interface, ...)											\
	friend class Type;																		\
																							\
	inline static Type* StaticClass()														\
	{																						\
		static Type MyClassType(Type::TypeGenerator<Interface>(FriendlyName, L ## #Interface));\
		return &MyClassType;																\
	}																						\
																							\
	inline static constexpr std::wstring_view FriendlyName = L ## #Interface;				\
																							\
private:																					\
	inline static Type* StaticRegisterClass = StaticClass();								\
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
	static void REFLECTION_GetPropertyPointer(void*);										\
																							\
public:

#define GENERATED_STRUCT_BODY(Struct, ...)													\
	friend class Type;																		\
																							\
public:																						\
	using Super = typename ReflectionMacros::SuperClassTypeDeclare<Struct>::Type;			\
	using This = Struct;																	\
																							\
	inline static Type* StaticClass()														\
	{																						\
		static Type MyClassType = Type::TypeGenerator<Struct>(FriendlyName);				\
		return &MyClassType;																\
	}																						\
																							\
	inline static constexpr std::wstring_view FriendlyName = L ## #Struct;					\
																							\
private:																					\
	inline static Type* StaticRegisterClass = StaticClass();								\
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
	static void REFLECTION_GetPropertyPointer(void*);										\
																							\
public:																						\
	inline operator SObject* () const														\
	{																						\
		return Cast<SObject>(*this);														\
	}

#define GENERATE_BODY(Class, ...)															\
Type* Class::StaticClass()																	\
{																							\
	static Type MyClassType(Type::TypeGenerator<This>(FriendlyName, L ## #Class));			\
	return &MyClassType;																	\
}

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
			.DeferredMemberType = Type::GetDeferredStaticClass<								\
			std::remove_const_t<std::remove_pointer_t<decltype(This::PropertyName)>>>(),	\
			.Setter = +[](SObject* _this, const void* _value)								\
			{ ReflectionMacros::Assign(dynamic_cast<This*>(_this)->PropertyName, _value); },\
			.Getter = +[](const SObject* _this) -> const void*								\
			{ return &dynamic_cast<const This*>(_this)->PropertyName; },					\
			.Setter_Struct = +[](void* _this, const void* _value) { ReflectionMacros		\
			::Assign(reinterpret_cast<This*>(_this)->PropertyName, _value); },				\
			.Getter_Struct = +[](const void* _this) -> const void*							\
			{ return &reinterpret_cast<const This*>(_this)->PropertyName; },				\
			.bIsPointer = std::is_pointer_v<PT>,											\
			.bIsConst = std::is_const_v<PT>,												\
			.bIsStatic = ReflectionMacros::IsStaticMember(&This::PropertyName),				\
		};																					\
		return &Generator;																	\
	}

#define interface struct __declspec(novtable)
#define implements virtual public 
#define gcnew SObjectDetails::GCNewBinder() << new