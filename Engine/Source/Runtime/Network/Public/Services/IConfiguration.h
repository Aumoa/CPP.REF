// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Json/JsonUtility.h"

class IConfiguration
{
public:
	virtual ~IConfiguration() noexcept = default;

	virtual std::shared_ptr<IConfiguration> GetSection(String sectionName) = 0;
	virtual nlohmann::json GetValue() = 0;

public:
	inline nlohmann::json GetValue(String sectionName)
	{
		return GetSection(sectionName)->GetValue();
	}
};