// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "NetworkApplication.h"
#include "Services/JsonConfiguration.h"
#include "Services/DefaultServiceCollection.h"
#include <csignal>

NetworkApplication* NetworkApplication::_sApp;
static constexpr LogCategory LogNet(TEXT("LogNet"));

NetworkApplication::NetworkApplication(String contentPath)
{
	_sApp = this;

	Log::Info(LogNet, TEXT("Content Directory: {}"), Environment::GetCurrentDirectory());

	DirectoryReference dir(Environment::GetCurrentDirectory());
	FileReference appsettings = dir.GetFile(TEXTF("{}/appsettings.json", contentPath));
	if (appsettings.IsExists())
	{
		String json = appsettings.ReadAllText();
		_configuration = std::make_unique<JsonConfiguration>(nlohmann::json::parse(json.AsCodepage()));
	}
	else
	{
		_configuration = std::make_unique<JsonConfiguration>(nlohmann::json());
	}

	_provider = std::make_unique<DefaultServiceCollection>();
}

NetworkApplication::~NetworkApplication() noexcept
{
}

int32 NetworkApplication::Run()
{
	return (int32)PlatformMisc::GuardedMain([this]()
	{
		signal(SIGINT, _trap_sigint);
		_shutdown = TaskCompletionSource<>::Create<int32>();
		return (int)_startup().GetResult();
	});
}

std::shared_ptr<IConfiguration> NetworkApplication::GetConfiguration()
{
	return _configuration;
}

IServiceCollection* NetworkApplication::GetServiceCollection()
{
	return _provider.get();
}

Task<> NetworkApplication::StartAsync(std::stop_token cancellationToken)
{
	co_await _provider->InitializeHostedServices(cancellationToken);
}

Task<> NetworkApplication::StopAsync(std::stop_token cancellationToken)
{
	co_await _provider->FinalizeHostedServices(cancellationToken);
}

Task<int32> NetworkApplication::_startup()
{
	co_await StartAsync(_ss.get_token());
	Log::Info(LogNet, TEXT("Application startup."));

	co_return co_await _shutdown.GetTask();
	Log::Info(LogNet, TEXT("Application has been shutdown."));
}

void NetworkApplication::_post_shutdown()
{
	Log::Info(LogNet, TEXT("Ctrl+C interrupted."));

	std::stop_source ss;
	Task<>::CancelAfter(ss, 5s);
	co_await StopAsync(ss.get_token());

	_shutdown.SetResult(0);
}

void NetworkApplication::_trap_sigint(int)
{
	_sApp->_post_shutdown();
}