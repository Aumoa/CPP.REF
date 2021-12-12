// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Materials/Material.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIShader.h"

GENERATE_BODY(SMaterial);

SMaterial::SMaterial(std::shared_ptr<IRHIDevice> InDevice, EBaseShaderCode InBaseShaderCode) : Super(InDevice)
	, BaseShaderCode(InBaseShaderCode)
	, Device(InDevice)
{
}

void SMaterial::Compile()
{
	ShaderCode = Device->CompileShader(this);
}

auto SMaterial::GetBaseShaderCode() -> EBaseShaderCode
{
	return BaseShaderCode;
}