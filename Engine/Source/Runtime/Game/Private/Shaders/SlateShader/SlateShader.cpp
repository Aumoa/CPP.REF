// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Shaders/SlateShader/SlateShader.h"
#include "Materials/Material.h"
#include "GameEngine.h"
#include "Draw/SlateWindowElementList.h"
#include "Layout/LayoutImpl.h"
#include "RHI/IRHIDeviceContext.h"
#include "RHI/IRHIShaderResourceView.h"
#include "Shaders/SlateShader/SlateShader.h"

#ifndef BYTE
#define BYTE uint8
#endif

#include "SlateShaderVS.hlsl.h"
#include "SlateShaderPS.hlsl.h"

SSlateShader::SSlateShader(IRHIDevice* device) : Super(device)
{
	_imageSourceRanges.emplace_back() =
	{
		.RangeType = ERHIDescriptorRangeType::ShaderResourceView,
		.NumDescriptors = 1,
		.BaseShaderRegister = 0,
		.RegisterSpace = 0,
	};

	_parameters =
	{
		// [0] SlateConstants
		{
			.Name = L"SlateConstants",
			.ElementType = ERHIShaderParameterType::ScalarParameterConstants,
			.Index = 0
		},
		// [1] SlateElements
		{
			.Name = L"SlateElements",
			.ElementType = ERHIShaderParameterType::ScalarParameterConstants,
			.Index = 1
		},
		// [2] ImageSource
		{
			.Name = L"ImageSource",
			.ElementType = ERHIShaderParameterType::DescriptorTable,
			.Index = 2
		},
	};

	_elements =
	{
		// [0] SlateConstants
		{
			.Type = ERHIShaderParameterType::ScalarParameterConstants,
			.ScalarConstantsParameter =
			{
				.ShaderRegister = 0,
				.Num32Bits = sizeof(Vector2) / 4,
			}
		},
		// [1] SlateElements
		{
			.Type = ERHIShaderParameterType::ScalarParameterConstants,
			.ScalarConstantsParameter =
			{
				.ShaderRegister = 1,
				.Num32Bits = sizeof(DrawElement) / 4,
			}
		},
		// [2] ImageSource
		{
			.Type = ERHIShaderParameterType::DescriptorTable,
			.DescriptorTable =
			{
				.NumDescriptorRanges = (uint32)_imageSourceRanges.size(),
				.pDescriptorRanges = _imageSourceRanges.data()
			}
		}
	};
}

std::span<uint8 const> SSlateShader::CompileVS()
{
	return pSlateShaderVS;
}

std::span<uint8 const> SSlateShader::CompilePS()
{
	return pSlateShaderPS;
}

auto SSlateShader::InitElements(const SlateWindowElementList& DrawElements) -> InitElementContext
{
	InitElementContext Ctx;

	// Init render elements.
	const std::vector<SlateDrawElement>& Elements = DrawElements.GetElements();
	Ctx.Shader = this;
	Ctx.DrawElementsRef = &Elements;
	Ctx.RenderElements.reserve(Elements.size());

	Ctx.NumDescriptors = 0;
	for (auto& Element : DrawElements.GetElements())
	{
		if (IsRenderElement(Element))
		{
			const SlateRenderTransform& RenderTransform = Element.Transform.GetAccumulatedRenderTransform();

			DrawElement& RenderElement = Ctx.RenderElements.emplace_back();
			RenderElement.M = RenderTransform.GetMatrix();
			RenderElement.AbsolutePosition = RenderTransform.GetTranslation();
			RenderElement.AbsoluteSize = Element.Transform.GetLocalSize();
			RenderElement.Depth = (float)Element.Layer;
			RenderElement.TexturePosition = Vector2::GetZero();
			RenderElement.TextureSize = Vector2::GetOneVector();

			Ctx.NumDescriptors += 1;
		}
	}

	return Ctx;
}

void SSlateShader::RenderElements(IRHIDeviceContext* InContext, const Vector2& ScreenSize, const InitElementContext& InitCtx)
{
	// Begin slate frame.
	InContext->SetGraphicsShader(InitCtx.Shader->GetShader());
	InContext->SetGraphicsRoot32BitConstants(0, ScreenSize, 0);

	auto RenderIt = InitCtx.RenderElements.begin();
	for (auto& Element : *InitCtx.DrawElementsRef)
	{
		if (IsRenderElement(Element))
		{
			InContext->SetGraphicsRoot32BitConstants(1, *RenderIt, 0);
			InContext->SetGraphicsRootShaderResourceView(2, Element.Brush.ImageSource, 0, 1);
			InContext->DrawInstanced(4, 1, 0, 0);
			++RenderIt;
		}
	}
}

bool SSlateShader::IsRenderElement(const SlateDrawElement& InElement) const
{
	return InElement.Brush.ImageSource && InElement.Transform.HasRenderTransform();
}