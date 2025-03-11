// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12TextFormat.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Global.h"
#include "D3D12RHI/D3D12Graphics.h"

ND3D12TextFormat::ND3D12TextFormat(String InFontFamilyName, float InFontSize, bool bBold, bool bItalic)
{
	auto& Graphics = ND3D12Global::GetDynamicRHI();
	HR(Graphics.GetWriteFactory()->CreateTextFormat(
		InFontFamilyName.c_str(),
		nullptr,
		bBold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
		bItalic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		InFontSize,
		L"en-US",
		&pTextFormat
	));
}

#endif