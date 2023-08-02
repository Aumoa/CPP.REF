// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if __INTELLISENSE__
#define ACLASS(...) export
#else
#define ACLASS(...) export
#endif

#if __INTELLISENSE__
#define GENERATED_BODY(...)
#else
#define GENERATED_BODY(...)
#endif

#if __INTELLISENSE__
#define AFUNCTION(...)
#else
#define AFUNCTION(...)
#endif