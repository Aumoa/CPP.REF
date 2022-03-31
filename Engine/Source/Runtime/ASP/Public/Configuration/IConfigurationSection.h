// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IConfiguration.h"

namespace libty::Asp::inline Configuration
{
	struct IConfigurationSection : implements(IConfiguration)
	{
		GENERATED_BODY(IConfigurationSection);

	public:
		virtual std::wstring GetKey() = 0;
		virtual std::wstring GetPath() = 0;
		virtual std::wstring GetValue() = 0;
	};
}