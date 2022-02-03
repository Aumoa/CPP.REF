// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkCommon.h"
#include "RHI/RHIInterfaces.h"

class SVkFactory;

class VULKAN_API SVkFactoryChild : implements SObject, implements IRHIFactoryChild
{
	GENERATED_BODY(SVkFactoryChild)

private:
	SPROPERTY(Owner)
	IRHIFactory* Owner = nullptr;

public:
	SVkFactoryChild(IRHIFactory* Owner);
	SVkFactoryChild(SVkFactory* Owner);

	virtual void Dispose() override;

	virtual IRHIFactory* GetFactory() override;

protected:
	virtual void Dispose(bool bDisposing);
};