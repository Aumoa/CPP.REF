// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "RHI/RHIShader.h"
#include "Draw/SlateWindowElementList.h"
#include <vector>
#include <span>

class SMaterial;
class SRHIDeviceContext;
class SRHIShaderDescriptorView;
struct SlateRenderTransform;

class GAME_API SSlateShader : public SRHIShader
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
	SMaterial* _material = nullptr;
	SRHIShaderDescriptorView* _shaderDescriptorView = nullptr;

	std::vector<RHIDescriptorRange> _imageSourceRanges;
	std::vector<RHIDescriptorRange> _fontFaceBufferRanges;

public:
	SSlateShader(SRHIDevice* device);

	virtual void Compile(SRHIVertexFactory* vertexDeclaration) override;
	virtual std::vector<RHIShaderParameterElement> GetShaderParameterDeclaration() const override;

	SMaterial* GetDefaultMaterial() const;
	std::vector<DrawElement> MakeElements(const std::vector<SSlateWindowElementList::GenericSlateElement>& elements) const;
	void RenderElements(SRHIDeviceContext* deviceContext, const Vector2& screenSize, SSlateWindowElementList* elements);

protected:
	virtual std::span<uint8 const> CompileVS() override;
	virtual std::span<uint8 const> CompilePS() override;
};