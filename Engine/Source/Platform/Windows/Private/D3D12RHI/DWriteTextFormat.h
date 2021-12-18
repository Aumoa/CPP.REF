// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DXGIFactoryChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHITextFormat.h"

class SDWriteTextFormat : public SDXGIFactoryChild, implements IRHITextFormat
{
	GENERATED_BODY(SDWriteTextFormat)

private:
	ComPtr<IDWriteTextFormat> _format;

public:
	SDWriteTextFormat(SDXGIFactory* InFactory, ComPtr<IDWriteTextFormat> format);

public:
	DECLARE_GETTER(IDWriteTextFormat, _format);
};