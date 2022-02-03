// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Materials/Material.h"
#include "RHI/RHIInterfaces.h"

GENERATE_BODY(SMaterial);

SMaterial::SMaterial(IRHIDevice* InDevice, EBaseShaderCode InBaseShaderCode) : Super(InDevice)
	, BaseShaderCode(InBaseShaderCode)
{
}

void SMaterial::Compile()
{
	//ShaderCode = Device->CompileShader(this);
}

auto SMaterial::GetBaseShaderCode() -> EBaseShaderCode
{
	return BaseShaderCode;
}