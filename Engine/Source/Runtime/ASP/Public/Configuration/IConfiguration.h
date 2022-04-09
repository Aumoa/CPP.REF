// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Asp
{
	struct IConfigurationSection;

	struct ASP_API IConfiguration : implements(SObject)
	{
		GENERATED_BODY(IConfiguration);

	public:
		virtual std::vector<IConfigurationSection* const> GetChildren() = 0;
		virtual IConfigurationSection* GetSection(std::wstring_view key) = 0;
	};
}