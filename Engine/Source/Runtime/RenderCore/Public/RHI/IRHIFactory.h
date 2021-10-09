// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIEnums.h"

interface IRHIAdapter;
interface IRHIDevice;
interface IRHISwapChain;
interface IWindowView;
interface IRHIFontCollection;
interface IRHITextFormat;

interface IRHIFactory : implements SObject
{
	virtual IRHIAdapter* GetAdapter(int32 index) = 0;
	virtual IRHIDevice* CreateDevice(IRHIAdapter* adapter) = 0;
	virtual IRHISwapChain* CreateSwapChain(IWindowView* window, IRHIDevice* device) = 0;
	virtual IRHIFontCollection* CreateFontCollection(const std::filesystem::path& path) = 0;
	virtual IRHITextFormat* CreateTextFormat(std::wstring_view fontFamilyName, IRHIFontCollection* fontCollection, ERHIFontWeight fontWeight, ERHIFontStyle fontStyle, ERHIFontStretch fontStretch, float fontSize, std::wstring_view localeName) = 0;

	RENDERCORE_API static IRHIFactory* CreateFactory(SObject* outer);
};