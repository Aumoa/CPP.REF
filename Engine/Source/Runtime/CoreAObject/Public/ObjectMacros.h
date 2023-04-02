// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS
#define EXPORT_CALL __stdcall
#else
#define EXPORT_CALL
#endif

#define __ATYPE_COMBINE_THREE_MACROS__(X, Y, Z) __ ## X ## __ ## Y ## __ ## Z ## __
#define ACLASS(...)

#define __ATYPE_GENERATED_BODY__(FileId, LineNumber) __ATYPE_COMBINE_THREE_MACROS__(GENERATED_BODY, FileId, LineNumber)
#define GENERATED_BODY(...) __ATYPE_GENERATED_BODY__(FILE_ID, __LINE__)