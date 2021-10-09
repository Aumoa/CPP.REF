// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "LogRenderCore.h"
#include "RHI/RHIStructures.h"
#include <variant>

interface IRHIDevice;
interface IRHITexture2D;
interface IRHIShaderResourceView;

class RENDERCORE_API SMaterialInterface : implements SObject
{
	GENERATED_BODY(SMaterialInterface)

protected:
	using ShaderVars = std::variant<float, Vector4, std::vector<uint8>, IRHITexture2D*>;

private:
	IRHIDevice* _device = nullptr;
	std::vector<ShaderVars> _shaderVars;

	uint8 _bParamsCached : 1 = false;
	uint8 _bShaderResourceViewsCached : 1 = false;

	std::vector<int32> _shaderMappingIdx;
	IRHIShaderResourceView* _srv = nullptr;

public:
	SMaterialInterface(IRHIDevice* device);

	virtual bool SetScalarParameterValueByIndex(int32 index, float value);
	virtual bool GetScalarParameterValueByIndex(int32 index, float* value);
	virtual bool SetVectorParameterValueByIndex(int32 index, const Vector4& value);
	virtual bool GetVectorParameterValueByIndex(int32 index, Vector4* value);
	virtual bool SetParameterCollectionValueByIndex(int32 index, int32 size, const void* value);
	virtual bool GetParameterCollectionValueByIndex(int32 index, int32* size, void* value);
	virtual bool SetTexture2DParameterValueByIndex(int32 index, IRHITexture2D* value);
	virtual bool GetTexture2DParameterValueByIndex(int32 index, IRHITexture2D** value);
	virtual int32 GetParameterMappingIndex(std::wstring_view parameterName);
	virtual std::vector<RHIMaterialParameterInfo> GetParametersInfo() = 0;
	virtual EMaterialBlendMode GetBlendMode() = 0;

	bool SetScalarParameterValueByName(std::wstring_view parameterName, float value)
	{
		int32 index = GetParameterMappingIndex(parameterName);
		if (index == -1)
		{
			SE_LOG(LogRHI, Error, L"Parameter name {} is not member of this material interface.", parameterName);
			return false;
		}
		return SetScalarParameterValueByIndex(index, value);
	}

	bool GetScalarParameterValueByName(std::wstring_view parameterName, float* value)
	{
		int32 index = GetParameterMappingIndex(parameterName);
		if (index == -1)
		{
			SE_LOG(LogRHI, Error, L"Parameter name {} is not member of this material interface.", parameterName);
			return false;
		}
		return GetScalarParameterValueByIndex(index, value);
	}

	bool SetVectorParameterValueByName(std::wstring_view parameterName, const Vector4& value)
	{
		int32 index = GetParameterMappingIndex(parameterName);
		if (index == -1)
		{
			SE_LOG(LogRHI, Error, L"Parameter name {} is not member of this material interface.", parameterName);
			return false;
		}
		return SetVectorParameterValueByIndex(index, value);
	}

	bool GetVectorParameterValueByName(std::wstring_view parameterName, Vector4* value)
	{
		int32 index = GetParameterMappingIndex(parameterName);
		if (index == -1)
		{
			SE_LOG(LogRHI, Error, L"Parameter name {} is not member of this material interface.", parameterName);
			return false;
		}
		return GetVectorParameterValueByIndex(index, value);
	}

	bool SetParameterCollectionValueByName(std::wstring_view parameterName, int32 size, const void* value)
	{
		int32 index = GetParameterMappingIndex(parameterName);
		if (index == -1)
		{
			SE_LOG(LogRHI, Error, L"Parameter name {} is not member of this material interface.", parameterName);
			return false;
		}
		return SetParameterCollectionValueByIndex(index, size, value);
	}

	bool GetParameterCollectionValueByName(std::wstring_view parameterName, int32* size, void* value)
	{
		int32 index = GetParameterMappingIndex(parameterName);
		if (index == -1)
		{
			SE_LOG(LogRHI, Error, L"Parameter name {} is not member of this material interface.", parameterName);
			return false;
		}
		return GetParameterCollectionValueByIndex(index, size, value);
	}

	bool SetTexture2DParameterValueByName(std::wstring_view parameterName, IRHITexture2D* value)
	{
		int32 index = GetParameterMappingIndex(parameterName);
		if (index == -1)
		{
			SE_LOG(LogRHI, Error, L"Parameter name {} is not member of this material interface.", parameterName);
			return false;
		}
		return SetTexture2DParameterValueByIndex(index, value);
	}

	bool GetTexture2DParameterValueByName(std::wstring_view parameterName, IRHITexture2D** value)
	{
		int32 index = GetParameterMappingIndex(parameterName);
		if (index == -1)
		{
			SE_LOG(LogRHI, Error, L"Parameter name {} is not member of this material interface.", parameterName);
			return false;
		}
		return GetTexture2DParameterValueByIndex(index, value);
	}

private:
	template<class T>
	bool SetParameterValue(int32 index, T&& value) requires requires { ShaderVars(std::forward<T>(value)); };
	template<class T>
	bool GetParameterValue(int32 index, T** value) requires requires { std::get<T>(std::declval<ShaderVars>()); };
	void CacheParamsStorage();
};