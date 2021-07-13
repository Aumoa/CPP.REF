// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "MaterialInstance.h"
#include "LogGame.h"

using enum ELogVerbosity;

using namespace std;

MaterialInstance::MaterialInstance(Material* source) : Super(source->GetShader())
	, _source(source)
{
	_BlendMode = source->GetBlendMode();
}

void MaterialInstance::SetScalarParameterValueByIndex(int32 index, float value)
{
	StoreValue(_storage, index, value);
}

float MaterialInstance::GetScalarParameterValueByIndex(int32 index) const
{
	float v;
	if (!HasVars<float>(index, v))
	{
		return Super::GetScalarParameterValueByIndex(index);
	}
	else
	{
		return v;
	}
}

void MaterialInstance::SetVector3ParameterValueByIndex(int32 index, const Vector3& value)
{
	StoreValue(_storage, index, value);
}

Vector3 MaterialInstance::GetVector3ParameterValueByIndex(int32 index) const
{
	Vector3 v;
	if (!HasVars<Vector3>(index, v))
	{
		return Super::GetVector3ParameterValueByIndex(index);
	}
	else
	{
		return v;
	}
}

void MaterialInstance::SetGraphicsParameterValue(RHIDeviceContext* dc, int32 index) const
{
	if (index >= _storage.size())
	{
		// Does not contains any parameter value.
		LogSystem::Log(LogMaterial, Warning, L"The default parameter does not setted at index [{}]. Can keep running, but material values are undefined.", index);
		return;
	}

	const ShaderVars& vars = _storage[index];
	if (vars.index() == variant_npos)
	{
		// There is no override value.
		Super::SetGraphicsParameterValue(dc, index);
	}
	else
	{
		SetGraphicsParameterValueWithVariant(dc, index, _storage[index]);
	}
}

template<class T>
bool MaterialInstance::HasVars(int32 index, T& outValue) const
{
	if (_storage.size() <= index)
	{
		return false;
	}
	else
	{
		const size_t vidx = _storage[index].index();
		if (vidx == variant_npos)
		{
			return false;
		}

		outValue = get<T>(_storage[index]);
		return true;
	}
}