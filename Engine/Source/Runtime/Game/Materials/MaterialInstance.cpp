// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "MaterialInstance.h"
#include "LogGame.h"

using enum ELogVerbosity;

MaterialInstance::MaterialInstance(Material* source) : Super(source->GetShader())
	, _source(source)
{
}

void MaterialInstance::SetScalarParameterValueByIndex(int32 index, float value)
{
	StoreValue(_storage, index, value);
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
	if (vars.index() == -1)
	{
		// There is no override value.
		Super::SetGraphicsParameterValue(dc, index);
	}
	else
	{
		SetGraphicsParameterValueWithVariant(dc, index, _storage[index]);
	}
}