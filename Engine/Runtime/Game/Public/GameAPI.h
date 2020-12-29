// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#ifdef __SC_RUNTIME_GAME_API__

#pragma warning(disable: 4250)

#include <mutex>

#define GAME_API __declspec(dllexport)

#define ScopedLock(x)\
std::lock_guard<std::mutex> lock_##x(x)
#define bind_delegate(FunctionName) TRawFunctionBind(this, &std::remove_reference_t<decltype(*this)>::FunctionName)
#define bind_delegate_obj(FunctionName) TObjectFunctionBind(this, &std::remove_reference_t<decltype(*this)>::FunctionName)

#define GEngine (*Engine::GetInstance())
#define GApplication (*Application::GetInstance())

template<class O, class T>
inline O* Cast(T* from)
{
	return dynamic_cast<O*>(from);
}

template<class O, class T>
inline const O* Cast(const T* from)
{
	return dynamic_cast<const O*>(from);
}

#else
#define GAME_API __declspec(dllimport)
#endif