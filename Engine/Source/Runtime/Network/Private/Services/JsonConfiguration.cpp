// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Services/JsonConfiguration.h"

JsonConfiguration::JsonConfiguration(nlohmann::json json)
	: _value(std::move(json))
{
	if (_value.is_array())
	{
		size_t index = 0;
		for (auto& it : _value)
		{
			_sections.emplace(TEXTF("{}", index++), std::make_shared<JsonConfiguration>(it));
		}
	}
	else if (_value.is_object())
	{
		for (auto it = _value.begin(); it != _value.end(); ++it)
		{
			_sections.emplace(TEXTF("{}", String(it.key())), std::make_shared<JsonConfiguration>(it.value()));
		}
	}
}

std::shared_ptr<IConfiguration> JsonConfiguration::GetSection(String sectionName)
{
	auto it = _sections.find(sectionName);
	if (it == _sections.end())
	{
		return nullptr;
	}
	return it->second;
}

nlohmann::json JsonConfiguration::GetValue()
{
	return _value;
}