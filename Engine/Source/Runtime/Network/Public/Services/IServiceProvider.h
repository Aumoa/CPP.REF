// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ServiceNotFoundException.h"
#include <typeinfo>
#include <any>

class IServiceProvider
{
public:
	virtual ~IServiceProvider() noexcept = default;

	virtual std::optional<std::any> GetConfig(const std::type_info& configType) = 0;
	virtual std::optional<std::any> GetService(const std::type_info& serviceType) = 0;

public:
	std::any GetRequiredConfig(const std::type_info& configType)
	{
		std::optional val = GetConfig(configType);
		if (val.has_value() == false)
		{
			throw ServiceNotFoundException(configType);
		}
		return val.value();
	}

	template<class T>
	std::optional<T> GetConfig()
	{
		std::optional v = GetConfig(typeid(T));
		if (v.has_value())
		{
			return std::any_cast<T>(v.value());
		}
		return std::nullopt;
	}

	template<class T>
	T GetRequiredConfig()
	{
		return std::any_cast<T>(GetRequiredConfig(typeid(T)));
	}

	std::any GetRequiredService(const std::type_info& serviceType)
	{
		std::optional val = GetService(serviceType);
		if (val.has_value() == false)
		{
			throw ServiceNotFoundException(serviceType);
		}
		return val.value();
	}

	template<class T>
	std::shared_ptr<T> GetService()
	{
		std::optional val = GetService(typeid(T));
		if (val.has_value())
		{
			return std::any_cast<std::shared_ptr<T>>(val.value());
		}
		return nullptr;
	}

	template<class T>
	std::shared_ptr<T> GetRequiredService()
	{
		return std::any_cast<std::shared_ptr<T>>(GetRequiredService(typeid(T)));
	}
};