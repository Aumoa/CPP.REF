// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/DirectXCommon.h"
#include "RHI/IRHIFactory.h"

class SDXGIAdapter;

class SDXGIFactory : implements SObject, implements IRHIFactory
{
	GENERATED_BODY(SDXGIFactory)

private:
	ComPtr<IDXGIFactory4> _factory;
	ComPtr<IDWriteFactory5> _writeFactory;
	std::vector<SDXGIAdapter*> _cachedAdapters;

public:
	SDXGIFactory();

	virtual IRHIAdapter* GetAdapter(int32 index) override;
	virtual IRHIDevice* CreateDevice(IRHIAdapter* adapter) override;
	virtual IRHISwapChain* CreateSwapChain(IWindowView* window, IRHIDevice* device) override;
	virtual IRHIFontCollection* CreateFontCollection(const std::filesystem::path& path) override;
	virtual IRHITextFormat* CreateTextFormat(std::wstring_view fontFamilyName, IRHIFontCollection* fontCollection, ERHIFontWeight fontWeight, ERHIFontStyle fontStyle, ERHIFontStretch fontStretch, float fontSize, std::wstring_view localeName) override;

public:
	DECLARE_GETTER(IDWriteFactory5, _writeFactory);
};