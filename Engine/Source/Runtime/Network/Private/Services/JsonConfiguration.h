// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Services/IConfiguration.h"
#include "Json/JsonUtility.h"

class JsonConfiguration : public IConfiguration
{
private:
	nlohmann::json _value;
	std::map<String, std::shared_ptr<JsonConfiguration>> _sections;

public:
	JsonConfiguration(nlohmann::json json);

	virtual std::shared_ptr<IConfiguration> GetSection(String sectionName) override;
	virtual nlohmann::json GetValue() override;
};