// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include "Exceptions/TargetInvocationException.h"
#include "Concepts/IDerivedFrom.h"
#include <vector>

class Object;
#define interface struct

// --------------- Supports Functions ---------------
template<class T>
inline T __SCLASS_Cast(void* rptr) requires
	std::is_pointer_v<T> &&
	IDerivedFrom<std::remove_const_t<std::remove_pointer_t<T>>, Object>
{
	auto* sptr = (Object*)rptr;
	return dynamic_cast<std::remove_const_t<T>>(sptr);
}

template<class T>
inline T& __SCLASS_Cast(void* rptr)
{
	T* nptr = reinterpret_cast<T*>(rptr);
	return *nptr;
}

template<class T>
inline T __SCLASS_Cast(void* rptr) requires
	std::is_reference_v<T>
{
	using TR = std::remove_reference_t<T>;
	TR* nptr = reinterpret_cast<TR*>(rptr);
	return std::forward<T>(reinterpret_cast<T>(*nptr));
}

template<class T>
inline const T& __SCLASS_Cast(const void* rptr)
{
	const T* nptr = reinterpret_cast<const T*>(rptr);
	return *nptr;
}

template<class T>
inline void* __SCLASS_Cast(const T& value)
{
	return &value;
}

template<IDerivedFrom<Object> T>
inline void* __SCLASS_Cast(T* value)
{
	auto* ovalue = dynamic_cast<Object*>(value);
	return reinterpret_cast<void*>(ovalue);
}

template<class T>
inline T* __SCLASS_GetRef(T& value)
{
	return &value;
}

template<IDerivedFrom<Object> T>
inline Object* __SCLASS_GetRef(T* value)
{
	return value;
}

template<class T>
inline void __SCLASS_SetRef(T& left, void* right)
{
	left = *reinterpret_cast<const T*>(right);
}

template<IDerivedFrom<Object> T>
inline void __SCLASS_SetRef(T*& left, void* right)
{
	left = dynamic_cast<T*>(reinterpret_cast<Object*>(right));
}

template<class TReturnType, class T, class... TArgs>
class __SCLASS_Invoke_template_function
{
	using function_t = TReturnType(T::*)(TArgs...);
	using static_function_t = TReturnType(*)(TArgs...);

	T* _self;
	function_t _fnc;

public:
	__SCLASS_Invoke_template_function(T* self, function_t fnc)
		: _self(self)
		, _fnc(fnc)
	{
	}

	template<class... TInvokes>
	void* operator ()(TInvokes&&... invokes)
	{
		if constexpr (std::same_as<TReturnType, void>)
		{
			(_self->*_fnc)(std::forward<TInvokes>(invokes)...);
			return nullptr;
		}
		else
		{
			static thread_local TReturnType ret;
			ret = (_self->*_fnc)(std::forward<TInvokes>(invokes)...);
			return __SCLASS_GetRef(ret);
		}
	}
};

// --------------------------------------------------

// --------------- Supports Macros ------------------

#define __COMBINE_THREE_MACROS(X, Y, Z) __ ## X ## __ ## Y ## __ ## Z ## __

#define __STYPE_BEGIN_NAMESPACE() namespace libty::reflect {
#define __STYPE_END_NAMESPACE() }

#define __STYPE_DEFINE_REFLEXPR(Class, Ctors, Props, Funcs) \
std::vector<constructor_t> reflexpr_ ## Class::constructors() \
{ \
	static auto ctors = Ctors; \
	return ctors; \
} \
std::vector<property_info_t> reflexpr_ ## Class::properties() \
{ \
	static auto props = Props; \
	return props; \
} \
std::vector<function_info_t> reflexpr_ ## Class::functions() \
{ \
	static auto funcs = Funcs; \
	return funcs; \
}

// --------------------------------------------------

// -------------------- SCLASS ----------------------

#define __SCLASS0(FileID, Line) __COMBINE_THREE_MACROS(LIBTY_SCLASS, FileID, Line)
#define SCLASS(...) __SCLASS0(__LIBTY_GENERATED_FILE_ID__, __LINE__);

#define __SCLASS_DECLARE_REFLEXPR(API, Class, Base, Interfaces) \
struct API reflexpr_ ## Class \
{ \
	using type_t = Class; \
	using is_class_t = int; \
	using super_t = reflexpr_ ## Base; \
	using interfaces_t = std::remove_reference_t<decltype(std::make_tuple Interfaces)>; \
	static constexpr String friendly_name() { return TEXT(#Class); } \
	static std::vector<constructor_t> constructors(); \
	static std::vector<property_info_t> properties(); \
	static std::vector<function_info_t> functions(); \
};

#define __SCLASS_DEFINE_REFLEXPR(Class, Ctors, Props, Funcs) \
__STYPE_DEFINE_REFLEXPR(Class, Ctors, Props, Funcs)

// --------------------------------------------------

// ----------------- GENERATED_BODY -----------------

#define __GENERATED_BODY0(FileID, Line) __COMBINE_THREE_MACROS(LIBTY_GENERATED_BODY, FileID, Line)
#define GENERATED_BODY(...) __GENERATED_BODY0(__LIBTY_GENERATED_FILE_ID__, __LINE__);

#define __SCLASS_DECLARE_GENERATED_BODY(Class, Base) \
	friend struct libty::reflect::reflexpr_ ## Class; \
\
private: \
	using This = Class; \
	using Super = Base; \
\
public: \
	static constexpr String FriendlyName = TEXT(#Class); \
\
protected: \
	virtual Type* Impl_GetType(); \
\
public: \
	static Type* StaticClass(); \
	static Type* InstantiatedClass(); \
\
private:

#define __SCLASS_DEFINE_GENERATED_BODY(Class, Base) \
static auto& s ## Class ## Token() \
{ \
	static auto sGen = libty::reflect::ClassTypeMetadata::Generate<reflexpr(Class)>(); \
	return sGen; \
} \
\
Type* Class::Impl_GetType() \
{ \
	return StaticClass(); \
} \
\
Type* Class::StaticClass() \
{ \
	static Type* sType = InstantiatedClass(); \
	static Type* sDummy = GenerateClassType(s ## Class ## Token()); \
	return sType; \
} \
\
Type* Class::InstantiatedClass() \
{ \
	return InstantiateClass(TEXT(#Class)); \
} \
\
static Type* s ## Static ## Class = Class::StaticClass();

#define __SINTERFACE_DECLARE_GENERATED_BODY(Interface, Base) \
	friend struct libty::reflect::reflexpr_ ## Interface; \
\
public: \
	static Type* StaticClass(); \
\
public:

#define __SINTERFACE_DEFINE_GENERATED_BODY(Interface, Base) \
Type* Interface::StaticClass() \
{ \
	static auto sToken = libty::reflect::ClassTypeMetadata::Generate<reflexpr(Interface)>(); \
	static Type* sType = GenerateClassType(sToken); \
	return sType; \
}

// --------------------------------------------------

// ------------------- SCONSTRUCTOR -----------------

#define SCONSTRUCTOR(...)

#define __STYPE_DECLARE_CONSTRUCTOR_INFO(Class, Arguments) \
static void* Invoke_constructor__ ## Arguments ## __(std::vector<void*> args);

#define __STYPE_DEFINE_CONSTRUCTOR_INFO(Class, Arguments, ...) \
void* Class::Invoke_constructor__ ## Arguments ## __(std::vector<void*> args) \
{ \
	Object* ptr = new Class __VA_ARGS__ ; \
	return ptr; \
}

// --------------------------------------------------

// ------------------- SPROPERTY --------------------

#define SPROPERTY(...)

#define __STYPE_DECLARE_PROPERTY_INFO(Name, DefaultValue, Access) \
static void* Invoke_getter__ ## Name ## __(void* ptr); \
static void Invoke_setter__ ## Name ## __(void* ptr, void* value);

#define __STYPE_DEFINE_PROPERTY_INFO(Class, Name, ...) \
void* Class::Invoke_getter__ ## Name ## __(void* ptr) \
{ \
	auto self = dynamic_cast<Class*>(reinterpret_cast<Object*>(ptr)); \
	return __SCLASS_GetRef(self->Name); \
} \
\
void Class::Invoke_setter__ ## Name ## __(void* ptr, void* value) \
{ \
	auto self = dynamic_cast<Class*>(reinterpret_cast<Object*>(ptr)); \
	__SCLASS_SetRef(self->Name, value); \
}

// ------------------- SFUNCTION --------------------

#define SFUNCTION(...)

#define __STYPE_DECLARE_FUNCTION_INFO(Name, Arguments) \
static void* Invoke_function__ ## Name ## __ ## Arguments ## __(void* self, std::vector<void*> args);

#define __STYPE_DEFINE_FUNCTION_INFO(Class, Name, SafeName, Arguments, ReturnType, ...) \
void* Class::Invoke_function__ ## Name ## __ ## SafeName ## __(void* self, std::vector<void*> args) \
{ \
	auto* oself = dynamic_cast<Class*>(reinterpret_cast<Object*>(self)); \
	return __SCLASS_Invoke_template_function(oself, (ReturnType(Class::*) Arguments )&Class::Name) __VA_ARGS__ ; \
}

// --------------------------------------------------

// ------------------- SINTERFACE -------------------

#define __SINTERFACE0(FileID, Line) __COMBINE_THREE_MACROS(LIBTY_SINTERFACE, FileID, Line)
#define SINTERFACE(...) __SINTERFACE0(__LIBTY_GENERATED_FILE_ID__, __LINE__);

#define __SINTERFACE_DECLARE_REFLEXPR(API, Interface, Base, Interfaces) \
struct API reflexpr_ ## Interface \
{ \
	using type_t = Interface; \
	using is_interface_t = int; \
	using super_t = reflexpr_ ## Base; \
	using interfaces_t = std::remove_reference_t<decltype(std::make_tuple Interfaces)>; \
	static constexpr String friendly_name() { return TEXT(#Interface); } \
	static std::vector<constructor_t> constructors(); \
	static std::vector<property_info_t> properties(); \
	static std::vector<function_info_t> functions(); \
};

#define __SINTERFACE_DEFINE_REFLEXPR(Interface, Ctors, Props, Funcs) \
__STYPE_DEFINE_REFLEXPR(Interface, Ctors, Props, Funcs)

// --------------------------------------------------

// --------------------- SENUM ----------------------

#define __SENUM0(FileID, Line)
#define SENUM(...)

// --------------------------------------------------