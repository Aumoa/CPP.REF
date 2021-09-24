// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include <variant>
#include "GameEnums.h"

class SRHIShader;
class SRHIDeviceContext;

class GAME_API SMaterial : implements SObject
{
	GENERATED_BODY(SMaterial)

protected:
	using ShaderVars = std::variant<float, Vector3>;

private:
	SRHIShader* _shader = nullptr;
	std::vector<RHIShaderParameterElement> _parametersDecl;
	std::vector<ShaderVars> _storage;

protected:
	EMaterialBlendMode _BlendMode = EMaterialBlendMode::Opaque;

public:
	SMaterial(SRHIShader* shader);

	virtual void SetupMaterial(SRHIDeviceContext* dc);

	virtual void SetScalarParameterValueByIndex(int32 index, float value);
	virtual void SetScalarParameterValueByName(std::wstring_view parameterName, float value);
	virtual float GetScalarParameterValueByIndex(int32 index) const;
	virtual float GetScalarParameterValueByName(std::wstring_view parameterName) const;

	virtual void SetVector3ParameterValueByIndex(int32 index, const Vector3& value);
	virtual void SetVector3ParameterValueByName(std::wstring_view parameterName, const Vector3& value);
	virtual Vector3 GetVector3ParameterValueByIndex(int32 index) const;
	virtual Vector3 GetVector3ParameterValueByName(std::wstring_view parameterName) const;

	inline SRHIShader* GetShader() const { return _shader; }
	inline EMaterialBlendMode GetBlendMode() const { return _BlendMode; }

	virtual int32 GetRootParameterMappingIndex(std::wstring_view parameterName) const { return -1; }
	virtual bool IgnoreParameterType(ERHIShaderParameterType type) const { return type == ERHIShaderParameterType::ParameterCollection_CameraConstants; }

protected:
	virtual void SetGraphicsParameterValue(SRHIDeviceContext* dc, int32 index) const;
	void SetGraphicsParameterValueWithVariant(SRHIDeviceContext* dc, int32 index, const ShaderVars& vars) const;

	template<class TStorage, class T>
	void StoreValue(TStorage& storage, int32 index, const T& value)
	{
		if (storage.size() <= index)
		{
			storage.resize((size_t)index + 1);
		}
		storage[index].template emplace<T>(value);
	}

public:
	inline void SetVector3ParameterValueByIndex(int32 index, const Color& value)
	{
		SetVector3ParameterValueByIndex(index, Vector3{ value.R, value.G, value.B });
	}
	inline void SetVector3ParameterValueByName(std::wstring_view parameterName, const Color& value)
	{
		SetVector3ParameterValueByName(parameterName, Vector3{ value.R, value.G, value.B });
	}
};