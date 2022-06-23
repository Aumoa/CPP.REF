// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
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

// --------------------------------------------------

// --------------- Supports Macros ------------------

#define __COMBINE_THREE_MACROS(X, Y, Z) __ ## X ## __ ## Y ## __ ## Z ## __

#define __SCLASS_BEGIN_NAMESPACE() namespace libty::reflect {
#define __SCLASS_END_NAMESPACE() }

// --------------------------------------------------

// -------------------- SCLASS ----------------------

#define __SCLASS0(FileID, Line) __COMBINE_THREE_MACROS(LIBTY_SCLASS, FileID, Line)
#define SCLASS(...) __SCLASS0(__LIBTY_GENERATED_FILE_ID__, __LINE__)

#define __SCLASS_DECLARE_REFLEXPR(Class) \
struct reflexpr_ ## Class \
{ \
	using is_class_t = int; \
	static constexpr String friendly_name = TEXT(#Class); \
	static std::vector<constructor_t> constructors; \
};

#define __SCLASS_DEFINE_REFLEXPR(Class, Ctors) \
std::vector<constructor_t> reflexpr_ ## Class::constructors = Ctors;

// --------------------------------------------------

// ----------------- GENERATED_BODY -----------------

#define __GENERATED_BODY0(FileID, Line) __COMBINE_THREE_MACROS(LIBTY_GENERATED_BODY, FileID, Line)
#define GENERATED_BODY(...) __GENERATED_BODY0(__LIBTY_GENERATED_FILE_ID__, __LINE__)

#define __SCLASS_DECLARE_GENERATED_BODY(Class, Base) \
private: \
	using This = Class; \
	using Super = Base; \
\
public: \
	virtual class Type* Impl_GetType() const; \
\
private:

#define __SCLASS_DEFINE_GENERATED_BODY(Class, Base) \
Type* Class::Impl_GetType() const \
{ \
	static const auto sToken = libty::reflect::ClassTypeMetadata::Generate<reflexpr(Class)>(); \
	static Type* GeneratedClass = GenerateClassType(sToken); \
	return GeneratedClass; \
}

// --------------------------------------------------

// ------------------- SCONSTRUCTOR -----------------

#define SCONSTRUCTOR(...)

#define __SCLASS_DECLARE_CONSTRUCTOR_INFO(API, Class, Arguments) \
API void* Invoke_constructor__ ## Class ## __ ## Arguments ## __(std::vector<void*> args);

#define __SCLASS_DEFINE_CONSTRUCTOR_INFO(Class, Arguments, ...) \
void* Invoke_constructor__ ## Class ## __ ## Arguments ## __(std::vector<void*> args) \
{ \
	Object* ptr = new Class __VA_ARGS__ ; \
	return ptr; \
}

// --------------------------------------------------

// ------------------- SPROPERTY --------------------

#define SPROPERTY(...)

#define __SCLASS_DECLARE_PROPERTY_INFO(Class, Name, DefaultValue, Access)

// --------------------------------------------------

#define SFUNCTION(...)