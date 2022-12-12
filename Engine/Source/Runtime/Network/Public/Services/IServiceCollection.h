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
	void Configure(IConfiguration* configuration)
	{
		Configure<T>(configuration->GetValue());
	}

	template<class T, class U = T>
	void AddSingleton()
	{
		AddSingleton(typeid(T), [](IServiceProvider* sp) -> std::any
		{
			return std::shared_ptr<T>(new U(sp));
		});
	}

	template<class T, class U>
	void AddSingleton(std::function<std::shared_ptr<U>(IServiceProvider*)> factory)
	{
		AddSingleton(typeid(T), [factory](IServiceProvider* sp) -> std::any
		{
			return (std::shared_ptr<T>)factory(sp);
		});
	}

	template<class T, class TLambda>
	void AddSingleton(TLambda&& lambda) requires
		std::invocable<TLambda, IServiceProvider*>
	{
		AddSingleton<T, typename std::invoke_result_t<TLambda, IServiceProvider*>::element_type>(std::forward<TLambda>(lambda));
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

	template<class T, class TLambda>
	void AddScoped(TLambda&& lambda) requires
		std::invocable<TLambda, IServiceProvider*>
	{
		AddScoped<T, typename std::invoke_result_t<TLambda, IServiceProvider*>::element_type>(std::forward<TLambda>(lambda));
	}

	template<class T>
	void AddHostedService()
	{
		AddHostedService(typeid(T), [](IServiceProvider* sp) -> std::any
		{
			return std::shared_ptr<IHostedService>(new T(sp));
		});
	}

	template<class T>
	void AddHostedService(std::function<std::shared_ptr<T>(IServiceProvider*)> factory)
	{
		AddHostedService(typeid(T), [factory](IServiceProvider* sp) -> std::any
		{
			return std::shared_ptr<IHostedService>(factory(sp));
		});
	}

	template<class TLambda>
	void AddHostedService(TLambda&& lambda) requires
		std::invocable<TLambda, IServiceProvider*> &&
		std::derived_from<typename std::invoke_result_t<TLambda, IServiceProvider*>::element_type, IHostedService>
	{
		AddHostedService<typename std::invoke_result_t<TLambda, IServiceProvider*>::element_type>(std::forward<TLambda>(lambda));
	}
};