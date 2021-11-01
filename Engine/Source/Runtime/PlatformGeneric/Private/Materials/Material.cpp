// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Materials/Material.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIShader.h"

SMaterial::SMaterial(IRHIDevice* InDevice, EBaseShaderCode InBaseShaderCode) : Super()
	, BaseShaderCode(InBaseShaderCode)
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