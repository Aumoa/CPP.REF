// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include <variant>
#include "GameEnums.h"

class RHIShader;
class RHIDeviceContext;

class Material : virtual public Object
{
public:
	using Super = Object;
	using ShaderVars = std::variant<float>;

private:
	RHIShader* _shader = nullptr;
	std::vector<RHIShaderParameterElement> _parametersDecl;
	std::vector<ShaderVars> _storage;

protected:
	EMaterialBlendMode _BlendMode = EMaterialBlendMode::Opaque;

public:
	Material(RHIShader* shader);

	virtual void SetupMaterial(RHIDeviceContext* dc);

	virtual void SetScalarParameterValueByIndex(int32 index, float value);
	virtual void SetScalarParameterValueByName(std::wstring_view parameterName, float value);
	inline RHIShader* GetShader() const { return _shader; }
	inline EMaterialBlendMode GetBlendMode() const { return _BlendMode; }

	virtual int32 GetRootParameterMappingIndex(std::wstring_view parameterName) const { return -1; }
	virtual bool IgnoreParameterType(ERHIShaderParameterType type) const { return type == ERHIShaderParameterType::ParameterCollection_CameraConstants; }

protected:
	virtual void SetGraphicsParameterValue(RHIDeviceContext* dc, int32 index) const;
	void SetGraphicsParameterValueWithVariant(RHIDeviceContext* dc, int32 index, const ShaderVars& vars) const;

	template<class TStorage, class T>
	void StoreValue(TStorage& storage, int32 index, const T& value)
	{
		if (storage.size() <= index)
		{
			storage.resize((size_t)index + 1);
		}
		storage[index].template emplace<T>(value);
	}
};