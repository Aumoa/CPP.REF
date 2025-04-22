// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	enum class RenderAPI
	{
		D3D12
	};
}

template<>
struct std::formatter<Ayla::RenderAPI, wchar_t> : public std::formatter<std::wstring_view, wchar_t>
{
	template<typename FormatContext>
	auto format(Ayla::RenderAPI api, FormatContext& ctx) const
	{
		switch (api)
		{
		case Ayla::RenderAPI::D3D12:
			return std::formatter<std::wstring_view, wchar_t>::format(L"D3D12", ctx);
		default:
			return std::formatter<std::wstring_view, wchar_t>::format(std::format(L"(RenderAPI){0}", (int)api), ctx);
		}
	}
};