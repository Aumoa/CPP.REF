// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IAwaiter.h"
#include <concepts>
#include <functional>
#include <exception>

template<class TTask>
concept ITask = requires
{
	{ std::declval<TTask>().GetAwaiter() };
	{ std::declval<typename TTask::Awaiter>() };

	// IAwaiter results interface.
	{ std::declval<TTask>().GetStatus() } -> std::convertible_to<ETaskStatus>;
	{ std::declval<TTask>().GetException() } -> std::convertible_to<std::exception_ptr>;
	{ std::declval<TTask>().ContinueWith(std::declval<std::function<void(TTask)>>(), std::declval<std::stop_token>()) };
	{ std::declval<TTask>().Wait() };
	{ std::declval<TTask>().GetResult() };
	{ std::declval<TTask>().IsCompleted() } -> std::convertible_to<bool>;
	{ std::declval<TTask>().IsCompletedSuccessfully() } -> std::convertible_to<bool>;
	{ std::declval<TTask>().IsCanceled() } -> std::convertible_to<bool>;
	{ std::declval<TTask>().IsFaulted() } -> std::convertible_to<bool>;
};