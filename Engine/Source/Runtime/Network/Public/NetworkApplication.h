// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class IConfiguration;
class IServiceCollection;
class IServiceProvider;
class DefaultServiceCollection;

class NETWORK_API NetworkApplication : public NonCopyable
{
private:
	static NetworkApplication* _sApp;
	std::stop_source _ss;
	TaskCompletionSource<int32> _shutdown;

	std::shared_ptr<IConfiguration> _configuration;
	std::unique_ptr<DefaultServiceCollection> _provider;

public:
	NetworkApplication(String contentPath = TEXT("."));
	virtual ~NetworkApplication() noexcept;

	int32 Run();

	IConfiguration* GetConfiguration();
	IServiceCollection* GetServiceCollection();
	IServiceProvider* GetServiceProvider();

protected:
	virtual Task<> StartAsync(std::stop_token cancellationToken);
	virtual Task<> StopAsync(std::stop_token cancellationToken);

private:
	Task<int32> _startup();
	void _post_shutdown();

	static void _trap_sigint(int);
};