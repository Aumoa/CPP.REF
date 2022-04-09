// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Attributes/ClassAttribute.h"

namespace Attributes::inline libty::inline Asp
{
	class ASP_API SClassAttributeRoute : public ::libty::SClassAttribute
	{
		GENERATED_BODY(SClassAttributeRoute);

	private:
		std::wstring _route;

	public:
		SClassAttributeRoute(std::wstring_view route);

		std::wstring GetRoute();
	};
}