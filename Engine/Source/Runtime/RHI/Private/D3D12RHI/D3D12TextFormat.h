// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHITextFormat.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12TextFormat : public NRHITextFormat
{
	ComPtr<IDWriteTextFormat> pTextFormat;

public:
	ND3D12TextFormat(String InFontFamilyName, float InFontSize, bool bBold, bool bItalic);

	inline IDWriteTextFormat* GetTextFormat() const { return pTextFormat.Get(); }
};

#endif