// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/DXGI/DXGIFactoryChild.h"
#include "RHI/IRHITextFormat.h"
#include "RHI/DirectXCommon.h"

class SDWriteTextFormat : public SDXGIFactoryChild, implements IRHITextFormat
{
	GENERATED_BODY(SDWriteTextFormat)

private:
	ComPtr<IDWriteTextFormat> _format;

public:
	SDWriteTextFormat(SDXGIFactory* factory, ComPtr<IDWriteTextFormat> format);

public:
	DECLARE_GETTER(IDWriteTextFormat, _format);
};