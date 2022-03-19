// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "RHI/RHIStructures.h"
#include "Diagnostics/LogCategory.h"
#include "Numerics/VectorInterface/Vector.h"
#include <variant>

struct IRHIDevice;
struct IRHITexture2D;
struct IRHIShaderResourceView;
struct IRHIDeviceContext;

DECLARE_LOG_CATEGORY(PLATFORMGENERIC_API, LogMaterials);

class PLATFORMGENERIC_API SMaterialInterface : implements SObject
{
	GENERATED_BODY(SMaterialInterface)

protected:
	using ShaderVars = std::variant<float, Vector4, std::vector<uint8>, IRHITexture2D*>;

private:
	SPROPERTY(Device)
	IRHIDevice* Device = nullptr;
	std::vector<ShaderVars> ShaderValues;

	uint8 bParamsCached : 1 = false;
	uint8 bShaderResourceViewsCached : 1 = false;

	std::vector<int32> ShaderMappingIndex;
	//SPROPERTY(SRV)
	//IRHIShaderResourceView* SRV = nullptr;

public:
	SMaterialInterface(IRHIDevice* InDevice);

	virtual bool SetScalarParameterValueByIndex(int32 Index, float value);
	virtual bool GetScalarParameterValueByIndex(int32 Index, float* value);
	virtual bool SetVectorParameterValueByIndex(int32 Index, const Vector4& value);
	virtual bool GetVectorParameterValueByIndex(int32 Index, Vector4* value);
	virtual bool SetParameterCollectionValueByIndex(int32 Index, int32 Size, const void* value);
	virtual bool GetParameterCollectionValueByIndex(int32 Index, int32* Size, void* value);
	virtual bool SetTexture2DParameterValueByIndex(int32 Index, IRHITexture2D* value);
	virtual bool GetTexture2DParameterValueByIndex(int32 Index, IRHITexture2D** value);
	virtual int32 GetParameterMappingIndex(std::wstring_view ParameterName);
	virtual std::vector<RHIMaterialParameterInfo> GetParametersInfo() = 0;
	virtual EMaterialBlendMode GetBlendMode() = 0;
	virtual void SetupCommands(IRHIDeviceContext* InContext) = 0;

	bool SetScalarParameterValueByName(std::wstring_view ParameterName, float Value)
	{
		int32 Index = GetParameterMappingIndex(ParameterName);
		if (Index == -1)
		{
			SE_LOG(LogMaterials, Error, L"Parameter name {} is not member of this material interface.", ParameterName);
			return false;
		}
		return SetScalarParameterValueByIndex(Index, Value);
	}

	bool GetScalarParameterValueByName(std::wstring_view ParameterName, float* Value)
	{
		int32 Index = GetParameterMappingIndex(ParameterName);
		if (Index == -1)
		{
			SE_LOG(LogMaterials, Error, L"Parameter name {} is not member of this material interface.", ParameterName);
			return false;
		}
		return GetScalarParameterValueByIndex(Index, Value);
	}

	bool SetVectorParameterValueByName(std::wstring_view ParameterName, const Vector4& Value)
	{
		int32 Index = GetParameterMappingIndex(ParameterName);
		if (Index == -1)
		{
			SE_LOG(LogMaterials, Error, L"Parameter name {} is not member of this material interface.", ParameterName);
			return false;
		}
		return SetVectorParameterValueByIndex(Index, Value);
	}

	bool GetVectorParameterValueByName(std::wstring_view ParameterName, Vector4* Value)
	{
		int32 Index = GetParameterMappingIndex(ParameterName);
		if (Index == -1)
		{
			SE_LOG(LogMaterials, Error, L"Parameter name {} is not member of this material interface.", ParameterName);
			return false;
		}
		return GetVectorParameterValueByIndex(Index, Value);
	}

	bool SetParameterCollectionValueByName(std::wstring_view ParameterName, int32 size, const void* Value)
	{
		int32 Index = GetParameterMappingIndex(ParameterName);
		if (Index == -1)
		{
			SE_LOG(LogMaterials, Error, L"Parameter name {} is not member of this material interface.", ParameterName);
			return false;
		}
		return SetParameterCollectionValueByIndex(Index, size, Value);
	}

	bool GetParameterCollectionValueByName(std::wstring_view ParameterName, int32* size, void* Value)
	{
		int32 Index = GetParameterMappingIndex(ParameterName);
		if (Index == -1)
		{
			SE_LOG(LogMaterials, Error, L"Parameter name {} is not member of this material interface.", ParameterName);
			return false;
		}
		return GetParameterCollectionValueByIndex(Index, size, Value);
	}

	bool SetTexture2DParameterValueByName(std::wstring_view ParameterName, IRHITexture2D* Value)
	{
		int32 Index = GetParameterMappingIndex(ParameterName);
		if (Index == -1)
		{
			SE_LOG(LogMaterials, Error, L"Parameter name {} is not member of this material interface.", ParameterName);
			return false;
		}
		return SetTexture2DParameterValueByIndex(Index, Value);
	}

	bool GetTexture2DParameterValueByName(std::wstring_view ParameterName, IRHITexture2D** Value)
	{
		int32 Index = GetParameterMappingIndex(ParameterName);
		if (Index == -1)
		{
			SE_LOG(LogMaterials, Error, L"Parameter name {} is not member of this material interface.", ParameterName);
			return false;
		}
		return GetTexture2DParameterValueByIndex(Index, Value);
	}

private:
	template<class T>
	bool SetParameterValue(int32 Index, T&& Value) requires requires { ShaderVars(std::forward<T>(Value)); };
	template<class T>
	bool GetParameterValue(int32 Index, T** Value) requires requires { std::get<T>(std::declval<ShaderVars>()); };
	void CacheParamsStorage();
};