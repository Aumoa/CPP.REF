// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include <array>
#include <vector>

class Object;

#define __COMBINE_THREE_MACROS(X, Y, Z) __ ## X ## __ ## Y ## __ ## Z ## __

#define __SCLASS0(FileID, Line) __COMBINE_THREE_MACROS(LIBTY_SCLASS, FileID, Line)
#define SCLASS(...) __SCLASS0(__LIBTY_GENERATED_FILE_ID__, __LINE__)

#define __GENERATED_BODY0(FileID, Line) __COMBINE_THREE_MACROS(LIBTY_GENERATED_BODY, FileID, Line)
#define GENERATED_BODY(...) __GENERATED_BODY0(__LIBTY_GENERATED_FILE_ID__, __LINE__)

#define __SCLASS_BEGIN_NAMESPACE() namespace libty::reflect {
#define __SCLASS_END_NAMESPACE() }

#define __SCLASS_DECLARE_REFLEXPR(Class, Ctors) \
struct reflexpr_ ## Class \
{ \
	using is_class_t = int; \
	static constexpr String friendly_name = TEXT(#Class); \
\
	using constructor_t = Object*(*)(std::vector<Object*>); \
	static constexpr auto constructors = Ctors; \
};

#define __SCLASS_DECLARE_GENERATED_BODY(Class, Base) \
private: \
	using This = Class; \
	using Super = Base; \
\
public: \
	virtual class Type* Impl_GetType() const; \
\
private:

#define __SCLASS_DEFINE_GENERATED_BODY(Class, Base)

#define SCONSTRUCTOR(...)

#define __SCLASS_DECLARE_CONSTRUCTOR_INFO(API, Class, Arguments) \
API Object* Invoke_constructor__ ## Class ## __ ## Arguments ## __(std::vector<Object*> args);

#define __SCLASS_DEFINE_CONSTRUCTOR_INFO(Class, Arguments) \
Object* Invoke_constructor__ ## Class ## __ ## Arguments ## __(std::vector<Object*> args) \
{ \
	return nullptr; \
}

#define SPROPERTY(...)
#define SFUNCTION(...)