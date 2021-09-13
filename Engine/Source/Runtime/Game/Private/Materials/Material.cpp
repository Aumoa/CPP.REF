// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Materials/Material.h"
#include "LogGame.h"
#include "RHI/RHIShader.h"
#include "RHI/RHIDeviceContext.h"

SMaterial::SMaterial(SRHIShader* shader) : Super()
	, _shader(shader)
	, _parametersDecl(shader->GetShaderParameterDeclaration())
{
}

void SMaterial::SetupMaterial(SRHIDeviceContext* dc)
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

#define checkIdxf(idx, cnt, ...) \
if (idx < 0 || idx >= (int32)(cnt)) \
{\
	SE_LOG(LogMaterial, Error, L"The parameter index[{}] is not valid on this shader program.", (idx));\
	return __VA_ARGS__;\
}

void SMaterial::SetScalarParameterValueByIndex(int32 index, float value)
{
	checkIdxf(index, _parametersDecl.size());
	if (_parametersDecl[index].Type != ERHIShaderParameterType::ScalarParameterConstants)
	{
		SE_LOG(LogMaterial, Error, L"The type of parameter at index[{}] is not ScalarParameterConstants type.", index);
		return;
	}
	
	StoreValue(_storage, index, value);
}

void SMaterial::SetScalarParameterValueByName(std::wstring_view parameterName, float value)
{
	const int32 index = GetRootParameterMappingIndex(parameterName);
	if (index == -1)
	{
		SE_LOG(LogMaterial, Error, L"The parameter name \"{}\" does not found from this shader program.", parameterName);
		return;
	}

	SetScalarParameterValueByIndex(index, value);
}

float SMaterial::GetScalarParameterValueByIndex(int32 index) const
{
	checkIdxf(index, _parametersDecl.size(), 0);
	return get<float>(_storage[index]);
}

float SMaterial::GetScalarParameterValueByName(std::wstring_view parameterName) const
{
	const int32 index = GetRootParameterMappingIndex(parameterName);
	if (index == -1)
	{
		SE_LOG(LogMaterial, Error, L"The parameter name \"{}\" does not found from this shader program.", parameterName);
		return 0;
	}
	return GetScalarParameterValueByIndex(index);
}

void SMaterial::SetVector3ParameterValueByIndex(int32 index, const Vector3& value)
{
	checkIdxf(index, _parametersDecl.size());
	if (_parametersDecl[index].Type != ERHIShaderParameterType::ScalarParameterConstants)
	{
		SE_LOG(LogMaterial, Error, L"The type of parameter at index[{}] is not Vector3ParameterConstants type.", index);
		return;
	}

	StoreValue(_storage, index, value);
}

void SMaterial::SetVector3ParameterValueByName(std::wstring_view parameterName, const Vector3& value)
{
	const int32 index = GetRootParameterMappingIndex(parameterName);
	if (index == -1)
	{
		SE_LOG(LogMaterial, Error, L"The parameter name \"{}\" does not found from this shader program.", parameterName);
		return;
	}

	SetVector3ParameterValueByIndex(index, value);
}

Vector3 SMaterial::GetVector3ParameterValueByIndex(int32 index) const
{
	checkIdxf(index, _parametersDecl.size(), 0.0f);
	return get<Vector3>(_storage[index]);
}

Vector3 SMaterial::GetVector3ParameterValueByName(std::wstring_view parameterName) const
{
	const int32 index = GetRootParameterMappingIndex(parameterName);
	if (index == -1)
	{
		SE_LOG(LogMaterial, Error, L"The parameter name \"{}\" does not found from this shader program.", parameterName);
		return 0.0f;
	}
	return GetVector3ParameterValueByIndex(index);
}

void SMaterial::SetGraphicsParameterValue(SRHIDeviceContext* dc, int32 index) const
{
	if (index >= _storage.size())
	{
		// Does not contains any parameter value.
		SE_LOG(LogMaterial, Warning, L"The default parameter does not setted at index [{}]. Can keep running, but material values are undefined.", index);
		return;
	}

	SetGraphicsParameterValueWithVariant(dc, index, _storage[index]);
}

void SMaterial::SetGraphicsParameterValueWithVariant(SRHIDeviceContext* dc, int32 index, const ShaderVars& vars) const
{
	[[maybe_unused]] float fvalue;
	[[maybe_unused]] Vector3 v3;

	switch (vars.index())
	{
	case 0:
		fvalue = get<float>(vars);
		dc->SetGraphicsRoot32BitConstants((uint32)index, 1, &fvalue, 0);
		break;
	case 1:
		v3 = get<Vector3>(vars);
		dc->SetGraphicsRoot32BitConstants((uint32)index, 3, &v3, 0);
		break;
	default:
		SE_LOG(LogMaterial, Warning, L"The default parameter does not setted at index [{}]. Can keep running, but material values are undefined.", index);
		break;
	}
}