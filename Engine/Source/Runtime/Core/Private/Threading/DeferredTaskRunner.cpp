// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Threading/DeferredTaskRunner.h"
#include <boost/asio/io_service.hpp>

std::unique_ptr Service = std::make_unique<boost::asio::io_service>();

void DeferredTaskRunner::RegisterRunner(std::function<void()> Runner)
{
	if (Service)
	{
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
	Service->poll();
}

void DeferredTaskRunner::Stop()
{
	Service->poll();
	Service.reset();
}