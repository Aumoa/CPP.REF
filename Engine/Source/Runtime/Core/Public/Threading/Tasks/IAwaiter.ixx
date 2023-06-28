// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.Threading:IAwaiter;

export import Core.Std;
export import Core.System;
export import :TaskStatus;

export class AwaiterBase;

export template<class TAwaiter>
concept IAwaiter = requires
{
	// C++ coroutine interface.
	{ std::declval<TAwaiter>().await_ready() } -> std::convertible_to<bool>;
	{ std::declval<TAwaiter>().await_suspend(std::declval<std::coroutine_handle<>>()) };
	{ std::declval<TAwaiter>().await_resume() };

	// IAwaiter results interface.
	{ std::declval<const TAwaiter>().GetStatus() } -> std::convertible_to<ETaskStatus>;
	{ std::declval<TAwaiter>().ContinueWith(std::declval<std::function<void(std::shared_ptr<AwaiterBase>)>>()) };
	{ std::declval<TAwaiter>().GetException() } -> std::convertible_to<std::exception_ptr>;
	{ std::declval<TAwaiter>().GetResult() };
	{ std::declval<TAwaiter>().Wait() };
	{ std::declval<TAwaiter>().WaitFor(std::declval<TimeSpan>()) } -> std::same_as<bool>;
	{ std::declval<TAwaiter>().WaitUntil(std::declval<DateTime>()) } -> std::same_as<bool>;

	// IAwaiter continutation interface.
	{ std::declval<TAwaiter>().Cancel() };
	{ std::declval<TAwaiter>().SetException(std::declval<std::exception_ptr>()) } -> std::convertible_to<bool>;

	// IAwaiter misc interface.
	{ std::declval<TAwaiter>().AddCancellationToken(std::declval<std::stop_token>()) };
	{ std::declval<TAwaiter>().AddConditionVariable(std::declval<std::function<bool()>>()) };
	{ std::declval<const TAwaiter>().IsCancellationRequested() } -> std::convertible_to<bool>;
};