// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Services/DefaultServiceCollection.h"

DefaultServiceCollection::DefaultServiceCollection()
{
}

void DefaultServiceCollection::Configure(const std::type_info& configType, std::any value)
{
	_entries[configType.hash_code()] =
	{
		.Type = EServiceType::Config,
		.Instanced = value,
		.Factory = nullptr
	};
}

void DefaultServiceCollection::AddSingleton(const std::type_info& serviceType, std::function<std::any(IServiceProvider*)> factory)
{
	_entries[serviceType.hash_code()] =
	{
		.Type = EServiceType::Singleton,
		.Instanced = std::nullopt,
		.Factory = factory
	};
}

void DefaultServiceCollection::AddScoped(const std::type_info& serviceType, std::function<std::any(IServiceProvider*)> factory)
{
	_entries[serviceType.hash_code()] =
	{
		.Type = EServiceType::Scoped,
		.Instanced = std::nullopt,
		.Factory = factory
	};
}

void DefaultServiceCollection::AddHostedService(const std::type_info& serviceType, std::function<std::any(IServiceProvider*)> factory)
{
	_entries[serviceType.hash_code()] =
	{
		.Type = EServiceType::Hosted,
		.Instanced = std::nullopt,
		.Factory = factory
	};
}

std::optional<std::any> DefaultServiceCollection::GetConfig(const std::type_info& configType)
{
	auto it = _entries.find(configType.hash_code());
	if (it == _entries.end())
	{
		return std::nullopt;
	}

	if (it->second.Type != EServiceType::Config)
	{
		return std::nullopt;
	}

	return it->second.Instanced;
}

std::optional<std::any> DefaultServiceCollection::GetService(const std::type_info& serviceType)
{
	auto it = _entries.find(serviceType.hash_code());
	if (it == _entries.end())
	{
		return std::nullopt;
	}

	switch (it->second.Type)
	{
		case EServiceType::Singleton:
		{
			std::unique_lock lck(it->second.Lck);
			if (it->second.Instanced.has_value() == false)
			{
				it->second.Instanced = it->second.Factory(this);
			}
			return it->second.Instanced;
		}
		case EServiceType::Scoped:
			return it->second.Factory(this);
	}

	return std::nullopt;
}

Task<> DefaultServiceCollection::InitializeHostedServices(std::stop_token cancellationToken)
{
	std::vector<Task<>> tasks;
	for (auto& [_, entry] : _entries)
	{
		if (entry.Type == EServiceType::Hosted)
		{
			auto ptr = std::any_cast<std::shared_ptr<IHostedService>>((entry.Instanced = entry.Factory(this)).value());
			tasks.emplace_back(ptr->StartAsync(cancellationToken));
		}
	}

	if (tasks.size() > 0)
	{
		co_await Task<>::WhenAll(tasks);
	}
}

Task<> DefaultServiceCollection::FinalizeHostedServices(std::stop_token cancellationToken)
{
	std::vector<Task<>> tasks;
	for (auto& [_, entry] : _entries)
	{
		if (entry.Type == EServiceType::Hosted)
		{
			auto ptr = std::any_cast<std::shared_ptr<IHostedService>>(entry.Instanced.value());
			tasks.emplace_back(ptr->StopAsync(cancellationToken));
		}
	}

	if (tasks.size() > 0)
	{
		co_await Task<>::WhenAll(tasks);
	}
}