// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Services/IServiceCollection.h"
#include "Services/IServiceProvider.h"

class DefaultServiceCollection : public IServiceCollection, public IServiceProvider
{
	enum class EServiceType
	{
		Config,
		Singleton,
		Scoped,
		Hosted
	};

	struct ServiceEntry
	{
		EServiceType Type;
		std::optional<std::any> Instanced;
		std::function<std::any(IServiceProvider*)> Factory;
		Spinlock Lck;
	};

private:
	Spinlock _lock;
	std::map<size_t, ServiceEntry> _entries;
	std::map<size_t, ServiceEntry> _hosts;

public:
	DefaultServiceCollection();

	virtual void Configure(const std::type_info& configType, std::any value) override;
	virtual void AddSingleton(const std::type_info& serviceType, std::function<std::any(IServiceProvider*)> factory) override;
	virtual void AddScoped(const std::type_info& serviceType, std::function<std::any(IServiceProvider*)> factory) override;
	virtual void AddHostedService(const std::type_info& serviceType, std::function<std::any(IServiceProvider*)> factory) override;

	virtual std::optional<std::any> GetConfig(const std::type_info& configType) override;
	virtual std::optional<std::any> GetService(const std::type_info& serviceType) override;

	Task<> InitializeHostedServices(std::stop_token cancellationToken);
	Task<> FinalizeHostedServices(std::stop_token cancellationToken);
};