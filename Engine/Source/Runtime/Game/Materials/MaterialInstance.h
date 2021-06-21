// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "Material.h"

class MaterialInstance : public Material
{
public:
	using Super = Material;

private:
	Material* _source = nullptr;
	std::vector<ShaderVars> _storage;

public:
	MaterialInstance(Material* source);

	virtual void SetScalarParameterValueByIndex(int32 index, float value) override;

	virtual int32 GetRootParameterMappingIndex(std::wstring_view parameterName) const override { return _source->GetRootParameterMappingIndex(parameterName); }
	virtual bool IgnoreParameterType(ERHIShaderParameterType type) const override { return _source->IgnoreParameterType(type); }

protected:
	virtual void SetGraphicsParameterValue(RHIDeviceContext* dc, int32 index) const override;
};