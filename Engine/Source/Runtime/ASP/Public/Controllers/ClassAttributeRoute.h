// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Attributes/ClassAttribute.h"

namespace libty::Asp::inline Controllers
{
	class ASP_API SClassAttributeRoute : public SClassAttribute
	{
		GENERATED_BODY(SClassAttributeRoute);

	private:
		std::wstring _route;

	public:
		SClassAttributeRoute(std::wstring_view route);

		std::wstring GetRoute();
	};
}