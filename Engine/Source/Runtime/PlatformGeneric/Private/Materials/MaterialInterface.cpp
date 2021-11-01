// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Materials/MaterialInterface.h"
#include "RHI/IRHIDevice.h"

SMaterialInterface::SMaterialInterface(IRHIDevice* InDevice) : Super()
	, Device(InDevice)
{
}

bool SMaterialInterface::SetScalarParameterValueByIndex(int32 Index, float Value)
{
	return SetParameterValue(Index, Value);
}

bool SMaterialInterface::GetScalarParameterValueByIndex(int32 Index, float* Value)
{
	float* ValueRef = nullptr;
	if (GetParameterValue(Index, &ValueRef))
	{
		*Value = *ValueRef;
		return true;
	}
	return false;
}

bool SMaterialInterface::SetVectorParameterValueByIndex(int32 Index, const Vector4& Value)
{
	return SetParameterValue(Index, Value);
}

bool SMaterialInterface::GetVectorParameterValueByIndex(int32 Index, Vector4* Value)
{
	Vector4* ValueRef = nullptr;
	if (GetParameterValue(Index, &ValueRef))
	{
		*Value = *ValueRef;
		return true;
	}
	return false;
}

bool SMaterialInterface::SetParameterCollectionValueByIndex(int32 Index, int32 Size, const void* Value)
{
	std::vector<uint8> blob((size_t)Size);
	memcpy(blob.data(), Value, blob.size());
	if (SetParameterValue(Index, std::move(blob)))
	{
		bShaderResourceViewsCached = false;
		return true;
	}
	return false;
}

bool SMaterialInterface::GetParameterCollectionValueByIndex(int32 Index, int32* Size, void* Value)
{
	std::vector<uint8>* blob = nullptr;
	if (GetParameterValue(Index, &blob))
	{
		if (Size)
		{
			*Size = (int32)blob->size();
		}
		if (Value)
		{
			memcpy(Value, blob->data(), blob->size());
		}
		return true;
	}
	return false;
}

bool SMaterialInterface::SetTexture2DParameterValueByIndex(int32 Index, IRHITexture2D* Value)
{
	if (SetParameterValue(Index, Value))
	{
		bShaderResourceViewsCached = false;
		return true;
	}
	return false;
}

bool SMaterialInterface::GetTexture2DParameterValueByIndex(int32 Index, IRHITexture2D** Value)
{
	IRHITexture2D** ValueRef = nullptr;
	if (GetParameterValue(Index, &ValueRef))
	{
		*Value = *ValueRef;
		return true;
	}
	return false;
}

int32 SMaterialInterface::GetParameterMappingIndex(std::wstring_view ParameterName)
{
	std::vector<RHIMaterialParameterInfo> Parameters = GetParametersInfo();
	for (size_t i = 0; i < Parameters.size(); ++i)
	{
		if (Parameters[i].Name == ParameterName)
		{
			return Parameters[i].Index;
		}
	}

	return -1;
}

template<class T>
bool SMaterialInterface::SetParameterValue(int32 Index, T&& Value) requires requires { ShaderVars(std::forward<T>(Value)); }
{
	CacheParamsStorage();

	if (Index >= (int32)ShaderValues.size())
	{
		SE_LOG(LogMaterials, Error, L"Parameter Index({}) is not valid in this material.");
		return false;
	}

	ShaderValues[Index] = std::forward<T>(Value);
	return true;
}

template<class T>
bool SMaterialInterface::GetParameterValue(int32 Index, T** Value) requires requires { std::get<T>(std::declval<ShaderVars>()); }
{
	CacheParamsStorage();

	if (Index >= (int32)ShaderValues.size())
	{
		SE_LOG(LogMaterials, Error, L"Parameter Index({}) is not valid in this material.");
		return false;
	}

	*Value = &std::get<T>(ShaderValues[Index]);
	return true;
}

void SMaterialInterface::CacheParamsStorage()
{
	if (!bParamsCached)
	{
		std::vector<RHIMaterialParameterInfo> declaration = GetParametersInfo();
		ShaderValues.resize(declaration.size());

		size_t tableViews = 0;
		ShaderMappingIndex.reserve(ShaderValues.size());

		for (size_t i = 0; i < declaration.size(); ++i)
		{
			switch (declaration[i].ElementType)
			{
			case ERHIShaderParameterType::DescriptorTable:
			case ERHIShaderParameterType::ParameterCollection:
				tableViews += 1;
				ShaderMappingIndex.emplace_back((int32)i);
				break;
			default:
				ShaderMappingIndex.emplace_back(-1);
			}
		}

		if (tableViews != 0)
		{
			SRV = Device->CreateShaderResourceView((int32)tableViews);
			bShaderResourceViewsCached = false;
		}

		bParamsCached = true;
	}
}