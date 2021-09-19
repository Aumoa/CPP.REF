// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "LaunchConfig.h"
#include "LogGameLauncher.h"

SLaunchConfig::SLaunchConfig() : Super()
{
}

void SLaunchConfig::Init()
{
	using json = nlohmann::json;

	std::ifstream jsonReader(L"Game/Content/GameLauncher/LaunchItems.json", std::ios::in);
	if (!jsonReader.is_open())
	{
		SE_LOG(LogGameLauncher, Fatal, L"Could not found launch configuration file.");
	}

	std::vector<char> chars;
	chars.resize((size_t)jsonReader.seekg(0, std::ios::end).tellg());
	jsonReader.seekg(0, std::ios::beg);

	jsonReader.read(chars.data(), chars.size());
	jsonReader.close();

	json js = json::parse(chars);
	for (auto& element : js)
	{
		auto& item = _elements.emplace_back();
		item.Name = ANSI_TO_WCHAR(element["Name"], 65001);
		item.Icon = ANSI_TO_WCHAR(element["Icon"], 65001);
		item.Redirect = ANSI_TO_WCHAR(element["AppLink"], 65001);
		SE_LOG(LogGameLauncher, Verbose, L"Item Detected: Name: {}, Icon: {}, Redirect: {}", item.Name, item.Icon.wstring(), item.Redirect.wstring());
	}
}