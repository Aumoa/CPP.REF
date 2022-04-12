// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace Attributes::inline libty::inline Asp
{
	class ASP_API SMethodAttributeHttpGet : extends(::libty::SMethodAttribute)
	{
		GENERATED_BODY(SMethodAttributeHttpGet);

	private:
		std::wstring _route;

	public:
		SMethodAttributeHttpGet(std::wstring_view route = L"");

		std::wstring GetRoute();
	};
}