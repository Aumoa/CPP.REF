// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkDeviceChild.h"

class VULKAN_API SVkShaderCodeBlob : public SVkDeviceChild, implements IRHIShaderCodeBlob
{
	GENERATED_BODY(SVkShaderCodeBlob)

private:
	std::vector<uint8> Binary;

public:
	SVkShaderCodeBlob(SVkDevice* Owner, std::vector<uint8> Binary);

	using Super::Dispose;

	virtual const void* GetBufferPointer() override;
	virtual size_t GetBufferSize() override;

protected:
	virtual void Dispose(bool bDisposing) override;
};