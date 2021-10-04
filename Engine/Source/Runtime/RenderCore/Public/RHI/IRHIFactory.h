// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIAdapter;
interface IRHIDevice;
interface IRHISwapChain;
interface IWindowView;

interface IRHIFactory : implements SObject
{
	virtual IRHIAdapter* GetAdapter(int32 index) = 0;
	virtual IRHIDevice* CreateDevice(IRHIAdapter* adapter) = 0;
	virtual IRHISwapChain* CreateSwapChain(IWindowView* window, IRHIDevice* device) = 0;

	RENDERCORE_API static IRHIFactory* CreateFactory(SObject* outer);
};