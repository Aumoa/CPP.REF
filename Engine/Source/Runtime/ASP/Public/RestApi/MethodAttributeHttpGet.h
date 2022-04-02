// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Attributes::inline libty::inline Asp
{
	class ASP_API SMethodAttributeHttpGet : public ::libty::SMethodAttribute
	{
		GENERATED_BODY(SMethodAttributeHttpGet);

	private:
		std::wstring _route;

	public:
		SMethodAttributeHttpGet(std::wstring_view route = L"");

		std::wstring GetRoute();
	};
}