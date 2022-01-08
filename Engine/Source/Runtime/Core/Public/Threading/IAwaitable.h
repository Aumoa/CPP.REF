// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <coroutine>
#include <memory>

template<class T>
interface IAwaitable : public std::enable_shared_from_this<IAwaitable<T>>
{
	virtual ~IAwaitable() noexcept {}

	virtual bool IsReady() const = 0;
	virtual void Wait() = 0;
	virtual T GetValue() = 0;
	virtual void Then(std::function<void(T)> Proc) = 0;

	virtual void SetRunner(std::future<void> Runner) = 0;
	virtual std::future<void> GetRunner() = 0;

	virtual bool await_ready() = 0;
	virtual void await_suspend(std::coroutine_handle<> Coroutine) noexcept = 0;
	virtual T await_resume() noexcept = 0;
};