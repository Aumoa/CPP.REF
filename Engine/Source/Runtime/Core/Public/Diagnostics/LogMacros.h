// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#define DEFINE_LOG_CATEGORY(LogName) \
\
struct LogName \
{ \
    static constexpr ::Ayla::String CategoryName = TEXT(#LogName); \
    \
    template<class... TArgs> \
    static inline void Log(::Ayla::LogVerbosity logLevel, ::Ayla::String format, TArgs&&... args) \
    { \
        ::Ayla::Debug::Log(CategoryName, logLevel, format, std::forward<TArgs>(args)...); \
    } \
    \
    template<class... TArgs> \
    static inline void Verbose(::Ayla::String format, TArgs&&... args) \
    { \
        ::Ayla::Debug::LogVerbose(CategoryName, format, std::forward<TArgs>(args)...); \
    } \
    \
    template<class... TArgs> \
    static inline void Info(::Ayla::String format, TArgs&&... args) \
    { \
        ::Ayla::Debug::LogInfo(CategoryName, format, std::forward<TArgs>(args)...); \
    } \
    \
    template<class... TArgs> \
    static inline void Warning(::Ayla::String format, TArgs&&... args) \
    { \
        ::Ayla::Debug::LogWarning(CategoryName, format, std::forward<TArgs>(args)...); \
    } \
    \
    template<class... TArgs> \
    static inline void Error(::Ayla::String format, TArgs&&... args) \
    { \
        ::Ayla::Debug::LogError(CategoryName, format, std::forward<TArgs>(args)...); \
    } \
    \
    template<class... TArgs> \
    static inline void Critical(::Ayla::String format, TArgs&&... args) \
    { \
        ::Ayla::Debug::LogCritical(CategoryName, format, std::forward<TArgs>(args)...); \
    } \
};
