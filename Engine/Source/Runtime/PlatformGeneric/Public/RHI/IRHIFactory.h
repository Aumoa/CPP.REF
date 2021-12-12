// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIEnums.h"
#include <filesystem>

interface IApplicationInterface;
interface IRHIAdapter;
interface IRHIDevice;
interface IRHISwapChain;
interface IWindowView;
interface IRHIFontCollection;
interface IRHITextLayout;
interface IRHITextFormat;

interface IRHIFactory
{
	virtual IApplicationInterface* GetApplication() = 0;
	virtual std::shared_ptr<IRHIAdapter> GetAdapter(int32 Index) = 0;
	virtual std::shared_ptr<IRHIDevice> CreateDevice(std::shared_ptr<IRHIAdapter> Adapter) = 0;
	virtual std::shared_ptr<IRHISwapChain> CreateSwapChain(std::shared_ptr<IRHIDevice> Device) = 0;
	virtual std::shared_ptr<IRHIFontCollection> CreateFontCollection(const std::filesystem::path& Path) = 0;
	virtual std::shared_ptr<IRHITextFormat> CreateTextFormat(std::wstring_view FontFamilyName, std::shared_ptr<IRHIFontCollection> FontCollection, ERHIFontWeight FontWeight, ERHIFontStyle FontStyle, ERHIFontStretch FontStretch, float FontSize, std::wstring_view LocaleName) = 0;
	virtual std::shared_ptr<IRHITextLayout> CreateTextLayout(std::shared_ptr<IRHITextFormat> Format, std::wstring_view Text, const Vector2& Layout) = 0;
};