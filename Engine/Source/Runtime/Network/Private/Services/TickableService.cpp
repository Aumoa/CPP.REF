// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Services/TickableService.h"
#include "Services/IServiceProvider.h"

constexpr LogCategory LogTick(TEXT("LogTick"));

TickableService::TickableService(IServiceProvider* provider)
{
	_config = provider->GetConfig<Configuration>().value_or(Configuration());
	_tcs = TaskCompletionSource<>::Create();
}

Task<> TickableService::StartAsync(std::stop_token cancellationToken)
{
	_wt = std::jthread(std::bind(&TickableService::_worker, this, std::placeholders::_1));
	return Task<>::CompletedTask();
}

Task<> TickableService::StopAsync(std::stop_token cancellationToken)
{
	_wt.request_stop();

	{
		std::unique_lock lck(_mtx);
		_cv.NotifyOne();
	}

	co_await _tcs.GetTask();
	_wt.detach();
}

void TickableService::_worker(std::stop_token cancellationToken)
{
	Log::Info(LogTick, TEXT("TickableService worker starting up."));

	DateTime last = DateTime::Now();

	while (cancellationToken.stop_requested() == false)
	{
		DateTime now = DateTime::Now();
		TimeSpan deltaTime = now - last;
		Tick(deltaTime);
		last = now;

		if (_config.TickInterval > 0)
		{
			TimeSpan interval = TimeSpan::FromSeconds(_config.TickInterval);
			{
				std::unique_lock lck(_mtx);
				_cv.WaitFor(lck, interval);
			}
		}
	}

	Log::Info(LogTick, TEXT("TickableService worker has been shutting down."));
	_tcs.SetResult();
}