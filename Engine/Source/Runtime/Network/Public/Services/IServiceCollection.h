// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IHostedService.h"
#include "IConfiguration.h"
#include <any>

class IServiceProvider;

class IServiceCollection
{
public:
	virtual ~IServiceCollection() noexcept = default;

public:
	virtual void Configure(const std::type_info& configType, std::any value) = 0;
	virtual void AddSingleton(const std::type_info& serviceType, std::function<std::any(IServiceProvider*)> factory) = 0;
	virtual void AddScoped(const std::type_info& serviceType, std::function<std::any(IServiceProvider*)> factory) = 0;
	virtual void AddHostedService(const std::type_info& serviceType, std::function<std::any(IServiceProvider*)> factory) = 0;

public:
	template<class T>
	void Configure(const T& value)
	{
		Configure(typeid(T), value);
	}

	template<class T>
	void Configure(const std::shared_ptr<IConfiguration>& configuration)
	{
		Configure<T>(configuration->GetValue());
	}

	template<class T, class U = T>
	void AddSingleton()
	{
		AddSingleton(typeid(T), [](IServiceProvider* sp) -> std::any
		{
			return std::make_shared<U>(sp);
		});
	}

	template<class T, class U>
	void AddSingleton(std::function<std::shared_ptr<U>(IServiceProvider*)> factory)
	{
		AddSingleton(typeid(T), [factory](IServiceProvider* sp) -> std::any
		{
			return factory(sp);
		});
	}

	template<class T, class U = T>
	void AddScoped()
	{
		AddScoped(typeid(T), [](IServiceProvider* sp) -> std::any
		{
			return std::make_shared<U>(sp);
		});
	}

	template<class T, class U>
	void AddScoped(std::function<std::shared_ptr<U>(IServiceProvider*)> factory)
	{
		AddScoped(typeid(T), [factory](IServiceProvider* sp) -> std::any
		{
			return factory(sp);
		});
	}

	template<class T, class U = T>
	void AddHostedService()
	{
		AddHostedService(typeid(T), [](IServiceProvider* sp) -> std::any
		{
			return std::shared_ptr<IHostedService>(new U(sp));
		});
	}

	template<class T, class U>
	void AddHostedService(std::function<std::shared_ptr<U>(IServiceProvider*)> factory)
	{
		AddHostedService(typeid(T), [factory](IServiceProvider* sp) -> std::any
		{
			return std::shared_ptr<IHostedService>(factory(sp));
		});
	}
};