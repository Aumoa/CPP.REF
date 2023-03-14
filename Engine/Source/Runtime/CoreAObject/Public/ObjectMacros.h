// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS
#define EXPORT_CALL __stdcall
#else
#define EXPORT_CALL
#endif