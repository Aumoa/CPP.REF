// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DWriteTextFormat.h"

SDWriteTextFormat::SDWriteTextFormat(SDXGIFactory* factory, ComPtr<IDWriteTextFormat> format) : Super(factory)
	, _format(std::move(format))
{
}