// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#ifdef __SC_RUNTIME_GAME_API__

#pragma warning(disable: 4250)

#include <mutex>

#define ScopedLock(x)\
std::lock_guard<std::mutex> lock_##x(x)
#define bind_delegate(FunctionName) TRawFunctionBind(this, &std::remove_reference_t<decltype(*this)>::FunctionName)
#define bind_delegate_obj(FunctionName) TObjectFunctionBind(this, &std::remove_reference_t<decltype(*this)>::FunctionName)

#define GEngine (*Engine::GetInstance())
#define GApplication (*Application::GetInstance())

#else
#endif

#undef GetMessage