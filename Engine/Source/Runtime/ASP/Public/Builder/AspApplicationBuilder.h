// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace libty::Asp::inline Builder
{
	class ASP_API SAspApplicationBuilder : virtual public SObject
	{
		GENERATED_BODY(SAspApplicationBuilder);
		friend class SAspApplication;

	private:
		SAspApplicationBuilder();
	};
}