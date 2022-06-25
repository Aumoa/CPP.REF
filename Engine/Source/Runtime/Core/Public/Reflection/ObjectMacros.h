// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include "Exceptions/TargetInvocationException.h"
#include <vector>

class Object;

// --------------- Supports Functions ---------------
template<class T>
inline T __SCLASS_Cast(void* rptr) requires
	std::is_pointer_v<T> &&
	std::derived_from<std::remove_const_t<std::remove_pointer_t<T>>, Object>
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
inline T* __SCLASS_GetRef(T& value)
{
	return &value;
}

template<std::derived_from<Object> T>
inline Object* __SCLASS_GetRef(T* value)
{
	return value;
}

template<class T>
inline void __SCLASS_SetRef(T& left, void* right)
{
	left = *reinterpret_cast<const T*>(right);
}

template<std::derived_from<Object> T>
inline void __SCLASS_SetRef(T* left, void* right)
{
	left = dynamic_cast<T*>(reinterpret_cast<Object*>(right));
}

// --------------------------------------------------

// --------------- Supports Macros ------------------

#define __COMBINE_THREE_MACROS(X, Y, Z) __ ## X ## __ ## Y ## __ ## Z ## __

#define __SCLASS_BEGIN_NAMESPACE() namespace libty::reflect {
#define __SCLASS_END_NAMESPACE() }

// --------------------------------------------------

// -------------------- SCLASS ----------------------

#define __SCLASS0(FileID, Line) __COMBINE_THREE_MACROS(LIBTY_SCLASS, FileID, Line)
#define SCLASS(...) __SCLASS0(__LIBTY_GENERATED_FILE_ID__, __LINE__)

#define __SCLASS_DECLARE_REFLEXPR(API, Class, Base) \
struct API reflexpr_ ## Class \
{ \
	using is_class_t = int; \
	using super_t = reflexpr_ ## Base; \
	static constexpr String friendly_name = TEXT(#Class); \
	static std::vector<constructor_t> constructors; \
	static std::vector<property_info_t> properties; \
};

#define __SCLASS_DEFINE_REFLEXPR(Class, Ctors, Props) \
std::vector<constructor_t> reflexpr_ ## Class::constructors = Ctors; \
std::vector<property_info_t> reflexpr_ ## Class::properties = Props;

// --------------------------------------------------

// ----------------- GENERATED_BODY -----------------

#define __GENERATED_BODY0(FileID, Line) __COMBINE_THREE_MACROS(LIBTY_GENERATED_BODY, FileID, Line)
#define GENERATED_BODY(...) __GENERATED_BODY0(__LIBTY_GENERATED_FILE_ID__, __LINE__)

#define __SCLASS_DECLARE_GENERATED_BODY(Class, Base) \
	friend struct libty::reflect::reflexpr_ ## Class; \
\
public: \
	using This = Class; \
	using Super = Base; \
\
protected: \
	virtual Type* Impl_GetType(); \
\
public: \
	static Type* StaticClass(); \
\
private:

#define __SCLASS_DEFINE_GENERATED_BODY(Class, Base) \
Type* Class::Impl_GetType() \
{ \
	return StaticClass(); \
} \
\
Type* Class::StaticClass() \
{ \
	static const auto sToken = libty::reflect::ClassTypeMetadata::Generate<reflexpr(Class)>(); \
	static Type* GeneratedClass = GenerateClassType(sToken); \
	return GeneratedClass; \
}

// --------------------------------------------------

// ------------------- SCONSTRUCTOR -----------------

#define SCONSTRUCTOR(...)

#define __SCLASS_DECLARE_CONSTRUCTOR_INFO(Class, Arguments) \
static void* Invoke_constructor__ ## Arguments ## __(std::vector<void*> args);

#define __SCLASS_DEFINE_CONSTRUCTOR_INFO(Class, Arguments, ...) \
void* Class::Invoke_constructor__ ## Arguments ## __(std::vector<void*> args) \
{ \
	Object* ptr = new Class __VA_ARGS__ ; \
	return ptr; \
}

// --------------------------------------------------

// ------------------- SPROPERTY --------------------

#define SPROPERTY(...)

#define __SCLASS_DECLARE_PROPERTY_INFO(Name, DefaultValue, Access) \
static void* Invoke_getter__ ## Name ## __(void* ptr); \
static void Invoke_setter__ ## Name ## __(void* ptr, void* value);

#define __SCLASS_DEFINE_PROPERTY_INFO(Class, Name, ...) \
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

// --------------------------------------------------

#define SFUNCTION(...)

#define __SCLASS_DECLARE_FUNCTION_INFO(Name, Arguments)
//static void* Invoke_function__ ## Arguments ## __(std::vector<void*> args);