// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NRHIGraphics;
class NRHICommandQueue;

struct RHI_API NRHIGlobal : public StaticClass
{
	static void InitDynamicRHI();
	static void ShutdownDynamicRHI();

	static NRHIGraphics& GetDynamicRHI() { return *DynamicRHI.get(); }
	static NRHICommandQueue& GetPrimaryCommandQueue() { return *PrimaryCommandQueue.get(); }

private:
	static std::shared_ptr<NRHIGraphics> DynamicRHI;
	static std::shared_ptr<NRHICommandQueue> PrimaryCommandQueue;
};