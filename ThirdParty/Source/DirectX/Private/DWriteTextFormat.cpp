// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DWriteTextFormat.h"

GENERATE_BODY(SDWriteTextFormat);

SDWriteTextFormat::SDWriteTextFormat(SDXGIFactory* InFactory, ComPtr<IDWriteTextFormat> format) : Super(InFactory)
	, _format(std::move(format))
{
}

void SDWriteTextFormat::Dispose(bool bDisposing)
{
	_format.Reset();

	Super::Dispose(bDisposing);
}