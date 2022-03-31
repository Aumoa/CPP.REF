// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/CommandLine.h"

namespace libty::Asp::inline Builder
{
	class SAspApplicationBuilder;

	class ASP_API SAspApplication : virtual public SObject
	{
		GENERATED_BODY(SAspApplication);

	private:
		SAspApplication();

	public:
		SAspApplicationBuilder* CreateBuilder(const CommandLine& args);
	};
}