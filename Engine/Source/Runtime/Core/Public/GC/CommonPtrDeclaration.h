// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#define AYLA__COMMON_PTR_DECLARATION \
\
inline T* Get() const noexcept { return m_Ptr; } \
inline T* operator ->() const noexcept { return Get(); }
