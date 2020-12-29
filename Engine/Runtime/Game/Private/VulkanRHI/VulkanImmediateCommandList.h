// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "VulkanCommandList.h"
#include "RHI/IRHIImmediateCommandList.h"

#include "VulkanMinimal.h"

interface IRHIDeferredCommandList;

class VulkanImmediateCommandList : public VulkanCommandList, virtual public IRHIImmediateCommandList
{
public:
	using Super = VulkanCommandList;
	using This = VulkanImmediateCommandList;

private:

public:
	VulkanImmediateCommandList();
	~VulkanImmediateCommandList() override;

	virtual void ExecuteCommandList(IRHIDeferredCommandList* deferredCommandList);
	virtual void Flush();
};