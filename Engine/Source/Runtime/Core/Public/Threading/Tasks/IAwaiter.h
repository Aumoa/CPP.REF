// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "TaskStatus.h"
#include <concepts>
#include <coroutine>
#include <exception>
#include <functional>
#include <memory>
#include <stop_token>

class AwaiterBase;

template<class TAwaiter>
concept IAwaiter = requires (TAwaiter& val, const TAwaiter& cval)
{
	// C++ coroutine interface.
	{ val.await_ready() } -> std::convertible_to<bool>;
	{ val.await_suspend(std::declval<std::coroutine_handle<>>()) };
	{ val.await_resume() };

	// IAwaiter results interface.
	{ cval.GetStatus() } -> std::convertible_to<ETaskStatus>;
	{ val.ContinueWith(std::declval<std::function<void(std::shared_ptr<AwaiterBase>)>>()) };
	{ val.GetException() } -> std::convertible_to<std::exception_ptr>;
	{ val.GetResult() };
	{ val.Wait() };

	// IAwaiter continutation interface.
	{ val.Cancel() };
	{ val.SetException(std::declval<std::exception_ptr>()) } -> std::convertible_to<bool>;

	// IAwaiter misc interface.
	{ val.AddCancellationToken(std::declval<std::stop_token>()) };
	{ val.AddConditionVariable(std::declval<std::function<bool()>>()) };
	{ cval.IsCancellationRequested() } -> std::convertible_to<bool>;
};