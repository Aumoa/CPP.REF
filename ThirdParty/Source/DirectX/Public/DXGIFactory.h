// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXCommon.h"
#include "RHI/IRHIFactory.h"

class SWindowsApplication;
class SDXGIAdapter;

class DIRECTX_API SDXGIFactory : implements SObject, implements IRHIFactory
{
	GENERATED_BODY(SDXGIFactory)

private:
	ComPtr<IDXGIFactory4> _factory;
	ComPtr<IDWriteFactory5> _writeFactory;

	SPROPERTY(_cachedAdapters)
	std::vector<SDXGIAdapter*> _cachedAdapters;

public:
	SDXGIFactory();

	virtual void Dispose() override;

	virtual IRHIAdapter* GetAdapter(int32 index) override;
	virtual IRHIDevice* CreateDevice(IRHIAdapter* adapter) override;
	virtual IRHISwapChain* CreateSwapChain(IRHIDevice* device) override;
	virtual IRHIFontCollection* CreateFontCollection(const std::filesystem::path& path) override;
	virtual IRHITextFormat* CreateTextFormat(std::wstring_view fontFamilyName, IRHIFontCollection* fontCollection, ERHIFontWeight fontWeight, ERHIFontStyle fontStyle, ERHIFontStretch fontStretch, float fontSize, std::wstring_view localeName) override;
	virtual IRHITextLayout* CreateTextLayout(IRHITextFormat* Format, std::wstring_view Text, const Vector2& Layout) override;

protected:
	virtual void Dispose(bool bDisposing);

public:
	DECLARE_GETTER(IDWriteFactory5, _writeFactory);
};