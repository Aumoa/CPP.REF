// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#define __COMBINE_THREE_MACROS(X, Y, Z) __ ## X ## __ ## Y ## __ ## Z ## __

#define __SCLASS0(FileID, Line) __COMBINE_THREE_MACROS(LIBTY_SCLASS, FileID, Line)
#define SCLASS(...) __SCLASS0(__LIBTY_GENERATED_FILE_ID__, __LINE__)

#define __GENERATED_BODY0(FileID, Line) __COMBINE_THREE_MACROS(LIBTY_GENERATED_BODY, FileID, Line)
#define GENERATED_BODY(...) __GENERATED_BODY0(__LIBTY_GENERATED_FILE_ID__, __LINE__)

#define __SCLASS_BEGIN_NAMESPACE() namespace libty::reflect {
#define __SCLASS_END_NAMESPACE() }

#define __SCLASS_DECLARE_REFLEXPR(Class) \
template<class T> requires std::same_as<T, Class> \
struct reflexpr_t<T> \
{ \
	static constexpr String friendly_name = TEXT(#Class); \
};

#define __SCLASS_DECLARE_GENERATED_BODY(Class, Base) \
private: \
	using This = Class; \
	using Super = Base; \
\
public: \
	virtual class Type* GetType() \
	{ \
		return nullptr; \
	} \
\
private: