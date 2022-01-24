// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Threading/DeferredTaskRunner.h"
#include <boost/asio/io_service.hpp>

static std::unique_ptr Service = std::make_unique<boost::asio::io_service>();
static std::atomic<size_t> Counter;

void DeferredTaskRunner::RegisterRunner(std::function<void()> Runner)
{
	if (Service)
	{
		++Counter;
		Service->post(Runner);
	}
	else
	{
		// Application will shutting down.
		Runner();
	}
}

void DeferredTaskRunner::Run()
{
	size_t Consume = Counter;
	size_t Actual = 0;
	for (size_t i = 0; i < Consume; ++i)
	{
		Actual += Service->poll_one();
	}
	Counter -= Actual;
}

void DeferredTaskRunner::Stop()
{
	Service->poll();
	Service.reset();
}