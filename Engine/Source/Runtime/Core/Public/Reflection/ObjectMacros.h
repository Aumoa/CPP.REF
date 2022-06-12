// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#define __COMBINE_THREE_MACROS(X, Y, Z) X ## Y ## Z

#define __SCLASS0(FileID, Line) __COMBINE_THREE_MACROS(__LIBTY_SCLASS, FileID, Line)
#define SCLASS(...) __SCLASS0(__LIBTY_GENERATED_FILE_ID__, __LINE__)

#define __GENERATED_BODY0(FileID, Line) __COMBINE_THREE_MACROS(__LIBTY_GENERATED_BODY, FileID, Line)
#define GENERATED_BODY(...) __GENERATED_BODY0(__LIBTY_GENERATED_FILE_ID__, __LINE__)