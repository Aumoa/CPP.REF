// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Materials/MaterialInterface.h"
#include "LogRenderCore.h"
#include "RHI/IRHIDevice.h"

SMaterialInterface::SMaterialInterface(IRHIDevice* device) : Super()
	, _device(device)
{
}

bool SMaterialInterface::SetScalarParameterValueByIndex(int32 index, float value)
{
	return SetParameterValue(index, value);
}

bool SMaterialInterface::GetScalarParameterValueByIndex(int32 index, float* value)
{
	float* valueRef = nullptr;
	if (GetParameterValue(index, &valueRef))
	{
		*value = *valueRef;
		return true;
	}
	return false;
}

bool SMaterialInterface::SetVectorParameterValueByIndex(int32 index, const Vector4& value)
{
	return SetParameterValue(index, value);
}

bool SMaterialInterface::GetVectorParameterValueByIndex(int32 index, Vector4* value)
{
	Vector4* valueRef = nullptr;
	if (GetParameterValue(index, &valueRef))
	{
		*value = *valueRef;
		return true;
	}
	return false;
}

bool SMaterialInterface::SetParameterCollectionValueByIndex(int32 index, int32 size, const void* value)
{
	std::vector<uint8> blob((size_t)size);
	memcpy(blob.data(), value, blob.size());
	if (SetParameterValue(index, std::move(blob)))
	{
		_bShaderResourceViewsCached = false;
		return true;
	}
	return false;
}

bool SMaterialInterface::GetParameterCollectionValueByIndex(int32 index, int32* size, void* value)
{
	std::vector<uint8>* blob = nullptr;
	if (GetParameterValue(index, &blob))
	{
		if (size)
		{
			*size = (int32)blob->size();
		}
		if (value)
		{
			memcpy(value, blob->data(), blob->size());
		}
		return true;
	}
	return false;
}

bool SMaterialInterface::SetTexture2DParameterValueByIndex(int32 index, IRHITexture2D* value)
{
	if (SetParameterValue(index, value))
	{
		_bShaderResourceViewsCached = false;
		return true;
	}
	return false;
}

bool SMaterialInterface::GetTexture2DParameterValueByIndex(int32 index, IRHITexture2D** value)
{
	IRHITexture2D** valueRef = nullptr;
	if (GetParameterValue(index, &valueRef))
	{
		*value = *valueRef;
		return true;
	}
	return false;
}

int32 SMaterialInterface::GetParameterMappingIndex(std::wstring_view parameterName)
{
	std::vector<RHIMaterialParameterInfo> parameters = GetParametersInfo();
	for (size_t i = 0; i < parameters.size(); ++i)
	{
		if (parameters[i].Name == parameterName)
		{
			return parameters[i].Index;
		}
	}

	return -1;
}

template<class T>
bool SMaterialInterface::SetParameterValue(int32 index, T&& value) requires requires { ShaderVars(std::forward<T>(value)); }
{
	CacheParamsStorage();

	if (index >= (int32)_shaderVars.size())
	{
		SE_LOG(LogMaterial, Error, L"Parameter index({}) is not valid in this material.");
		return false;
	}

	_shaderVars[index] = std::forward<T>(value);
	return true;
}

template<class T>
bool SMaterialInterface::GetParameterValue(int32 index, T** value) requires requires { std::get<T>(std::declval<ShaderVars>()); }
{
	CacheParamsStorage();

	if (index >= (int32)_shaderVars.size())
	{
		SE_LOG(LogMaterial, Error, L"Parameter index({}) is not valid in this material.");
		return false;
	}

	*value = &std::get<T>(_shaderVars[index]);
	return true;
}

void SMaterialInterface::CacheParamsStorage()
{
	if (!_bParamsCached)
	{
		std::vector<RHIMaterialParameterInfo> declaration = GetParametersInfo();
		_shaderVars.resize(declaration.size());

		size_t tableViews = 0;
		_shaderMappingIdx.reserve(_shaderVars.size());

		for (size_t i = 0; i < declaration.size(); ++i)
		{
			switch (declaration[i].ElementType)
			{
			case ERHIShaderParameterType::DescriptorTable:
			case ERHIShaderParameterType::ParameterCollection:
				tableViews += 1;
				_shaderMappingIdx.emplace_back((int32)i);
				break;
			default:
				_shaderMappingIdx.emplace_back(-1);
			}
		}

		if (tableViews != 0)
		{
			_srv = _device->CreateShaderResourceView((int32)tableViews);
			_bShaderResourceViewsCached = false;
		}

		_bParamsCached = true;
	}
}