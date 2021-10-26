// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Materials/Material.h"
#include "Draw/SlateWindowElementList.h"
#include <vector>
#include <span>

interface IRHIDeviceContext;
struct SlateRenderTransform;

class GAME_API SSlateShader : public SMaterial
{
	GENERATED_BODY(SSlateShader)

public:
	struct alignas(4) DrawElement
	{
		Matrix2x2 M;
		Vector2 AbsolutePosition;
		Vector2 AbsoluteSize;
		Vector2 TexturePosition;
		Vector2 TextureSize;
		float Depth;
		Vector3 pad;
	};

	enum class ESlateRenderMode
	{
		ImageSource,
		Glyph,
	};

private:
	std::vector<uint8> _vscode;
	std::vector<uint8> _pscode;

	std::vector<RHIMaterialParameterInfo> _parameters;
	std::vector<RHIShaderParameterElement> _elements;

	std::vector<RHIDescriptorRange> _imageSourceRanges;
	std::vector<RHIDescriptorRange> _fontFaceBufferRanges;

public:
	SSlateShader(IRHIDevice* device);

	virtual std::vector<RHIMaterialParameterInfo> GetParametersInfo() override { return _parameters; }
	virtual EMaterialBlendMode GetBlendMode() override { return EMaterialBlendMode::UI; }

	virtual std::vector<RHIShaderParameterElement> GetShaderParameterDeclaration() override { return _elements; }
	virtual std::span<uint8 const> CompileVS() override;
	virtual std::span<uint8 const> CompilePS() override;

	SMaterial* GetDefaultMaterial() const;
	std::vector<DrawElement> MakeElements(const std::vector<SlateDrawElement>& elements) const;
	void RenderElements(IRHIDeviceContext* deviceContext, const Vector2& screenSize, const SlateWindowElementList& elements);
};