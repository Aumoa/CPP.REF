// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Materials/MaterialInstance.h"
#include "LogGame.h"

SMaterialInstance::SMaterialInstance(SMaterial* source) : Super(source->GetShader())
	, _source(source)
{
	_BlendMode = source->GetBlendMode();
}

void SMaterialInstance::SetScalarParameterValueByIndex(int32 index, float value)
{
	StoreValue(_storage, index, value);
}

float SMaterialInstance::GetScalarParameterValueByIndex(int32 index) const
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

void SMaterialInstance::SetVector3ParameterValueByIndex(int32 index, const Vector3& value)
{
	StoreValue(_storage, index, value);
}

Vector3 SMaterialInstance::GetVector3ParameterValueByIndex(int32 index) const
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

void SMaterialInstance::SetGraphicsParameterValue(SRHIDeviceContext* dc, int32 index) const
{
	if (index >= _storage.size())
	{
		// Does not contains any parameter value.
		SE_LOG(LogMaterial, Warning, L"The default parameter does not setted at index [{}]. Can keep running, but material values are undefined.", index);
		return;
	}

	const ShaderVars& vars = _storage[index];
	if (vars.index() == std::variant_npos)
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
bool SMaterialInstance::HasVars(int32 index, T& outValue) const
{
	if (_storage.size() <= index)
	{
		return false;
	}
	else
	{
		const size_t vidx = _storage[index].index();
		if (vidx == std::variant_npos)
		{
			return false;
		}

		outValue = std::get<T>(_storage[index]);
		return true;
	}
}