// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Material.h"
#include "LogGame.h"
#include "RHI/RHIShader.h"
#include "RHI/RHIDeviceContext.h"

using enum ELogVerbosity;

using namespace std;

Material::Material(RHIShader* shader) : Super()
	, _shader(shader)
	, _parametersDecl(shader->GetShaderParameterDeclaration())
{
}

void Material::SetupMaterial(RHIDeviceContext* dc)
{
	for (size_t i = 0; i < _parametersDecl.size(); ++i)
	{
		if (IgnoreParameterType(_parametersDecl[i].Type))
		{
			continue;
		}

		SetGraphicsParameterValue(dc, (int32)i);
	}
}

void Material::SetScalarParameterValueByIndex(int32 index, float value)
{
	if (index < 0 || index >= (int32)_parametersDecl.size())
	{
		LogSystem::Log(LogMaterial, Error, L"The parameter index[{}] is not valid on this shader program.", index);
		return;
	}

	if (_parametersDecl[index].Type != ERHIShaderParameterType::ScalarParameterConstants)
	{
		LogSystem::Log(LogMaterial, Error, L"The type of parameter at index[{}] is not ScalarParameterConstants type.", index);
		return;
	}
	
	StoreValue(_storage, index, value);
}

void Material::SetScalarParameterValueByName(wstring_view parameterName, float value)
{
	const int32 index = GetRootParameterMappingIndex(parameterName);
	if (index == -1)
	{
		LogSystem::Log(LogMaterial, Error, L"The parameter name \"{}\" does not found from this shader program.", parameterName);
		return;
	}

	SetScalarParameterValueByIndex(index, value);
}

void Material::SetGraphicsParameterValue(RHIDeviceContext* dc, int32 index) const
{
	if (index >= _storage.size())
	{
		// Does not contains any parameter value.
		LogSystem::Log(LogMaterial, Warning, L"The default parameter does not setted at index [{}]. Can keep running, but material values are undefined.", index);
		return;
	}

	SetGraphicsParameterValueWithVariant(dc, index, _storage[index]);
}

void Material::SetGraphicsParameterValueWithVariant(RHIDeviceContext* dc, int32 index, const ShaderVars& vars) const
{
	[[maybe_unused]] float fvalue;
	switch (vars.index())
	{
	case 0:
		fvalue = get<float>(vars);
		dc->SetGraphicsRoot32BitConstants((uint32)index, 1, &fvalue, 0);
		break;
	default:
		LogSystem::Log(LogMaterial, Warning, L"The default parameter does not setted at index [{}]. Can keep running, but material values are undefined.", index);
		break;
	}
}