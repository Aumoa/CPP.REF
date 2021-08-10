// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "Material.h"

class GAME_API MaterialInstance : public Material
{
public:
	using Super = Material;

private:
	Material* _source = nullptr;
	std::vector<ShaderVars> _storage;

public:
	MaterialInstance(Material* source);

	virtual void SetScalarParameterValueByIndex(int32 index, float value) override;
	virtual float GetScalarParameterValueByIndex(int32 index) const override;
	virtual void SetVector3ParameterValueByIndex(int32 index, const Vector3& value) override;
	virtual Vector3 GetVector3ParameterValueByIndex(int32 index) const override;

	virtual int32 GetRootParameterMappingIndex(std::wstring_view parameterName) const override { return _source->GetRootParameterMappingIndex(parameterName); }
	virtual bool IgnoreParameterType(ERHIShaderParameterType type) const override { return _source->IgnoreParameterType(type); }

protected:
	virtual void SetGraphicsParameterValue(RHIDeviceContext* dc, int32 index) const override;

private:
	template<class T>
	bool HasVars(int32 index, T& outValue) const;
};