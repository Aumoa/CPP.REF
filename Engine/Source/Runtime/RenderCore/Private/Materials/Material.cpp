// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Materials/Material.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIShader.h"

SMaterial::SMaterial(IRHIDevice* device) : Super(device)
	, _device(device)
{
}

void SMaterial::Compile(SVertexFactory* vertexDeclaration)
{
	_shader = _device->CompileMaterial(this, vertexDeclaration);
	_shader->SetOuter(this);
}