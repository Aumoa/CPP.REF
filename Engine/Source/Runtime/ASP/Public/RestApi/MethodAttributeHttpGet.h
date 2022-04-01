// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace libty::Asp::inline RestApi
{
	class ASP_API SMethodAttributeHttpGet : public SMethodAttribute
	{
		GENERATED_BODY(SMethodAttributeHttpGet);

	private:
		std::wstring _route;

	public:
		SMethodAttributeHttpGet(std::wstring_view route = L"");

		std::wstring GetRoute();
	};
}