// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "RHI/RHIShader.h"
#include <vector>
#include <span>

class Material;
class RHIDeviceContext;
class SlateWindowElementList;
class RHIShaderDescriptorView;
struct SlateRenderTransform;

class GAME_API SlateShader : public RHIShader
{
	GENERATED_BODY(SlateShader)

public:
	struct DrawElement
	{
		Matrix2x2 M;
		Vector2 AbsolutePosition;
		Vector2 AbsoluteSize;
		float Depth;
		Vector3 pad;
	};

private:
	std::vector<uint8> _vscode;
	std::vector<uint8> _pscode;
	Material* _material = nullptr;
	RHIShaderDescriptorView* _shaderDescriptorView = nullptr;
	std::vector<RHIDescriptorRange> _ranges;

public:
	SlateShader(RHIDevice* device);

	virtual void Compile(RHIVertexFactory* vertexDeclaration) override;
	virtual std::vector<RHIShaderParameterElement> GetShaderParameterDeclaration() const override;

	Material* GetDefaultMaterial() const;
	DrawElement MakeElement(const SlateRenderTransform& geometry, const Vector2& localSize, float depth) const;
	void RenderElements(RHIDeviceContext* deviceContext, const Vector2& screenSize, SlateWindowElementList* elements);

protected:
	virtual std::span<uint8 const> CompileVS() override;
	virtual std::span<uint8 const> CompilePS() override;
};