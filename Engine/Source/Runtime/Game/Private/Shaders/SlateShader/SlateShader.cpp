// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Shaders/SlateShader/SlateShader.h"
#include "Materials/Material.h"
#include "GameEngine.h"
#include "Draw/SlateWindowElementList.h"
#include "EngineSubsystems/GameRenderSystem.h"

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
		.BaseShaderRegister = 1,
		.RegisterSpace = 0,
	};

	_fontFaceBufferRanges.emplace_back() =
	{
		.RangeType = ERHIDescriptorRangeType::ShaderResourceView,
		.NumDescriptors = 1,
		.BaseShaderRegister = 2,
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
			.ElementType = ERHIShaderParameterType::StructuredBuffer,
			.Index = 1
		},
		// [2] ImageSource
		{
			.Name = L"ImageSource",
			.ElementType = ERHIShaderParameterType::DescriptorTable,
			.Index = 2
		},
		// [3] RenderMode
		{
			.Name = L"RenderMode",
			.ElementType = ERHIShaderParameterType::ScalarParameterConstants,
			.Index = 3
		}
	};

	_elements =
	{
		// [0] SlateConstants
		{
			.Type = ERHIShaderParameterType::ScalarParameterConstants,
			.ScalarConstantsParameter =
			{
				.ShaderRegister = 0,
				.Num32Bits = 2,
			}
		},
		// [1] SlateElements
		{
			.Type = ERHIShaderParameterType::StructuredBuffer,
			.StructuredBuffer =
			{
				.ShaderRegister = 0,
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
		},
		// [3] RenderMode
		{
			.Type = ERHIShaderParameterType::ScalarParameterConstants,
			.ScalarConstantsParameter =
			{
				.ShaderRegister = 1,
				.Num32Bits = 1
			}
		}
	};
}

auto SSlateShader::MakeElements(const std::vector<SSlateWindowElementList::GenericSlateElement>& elements) const -> std::vector<DrawElement>
{
	//SFontCachingManager* fontCachingMgr = GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetFontCachingManager();

	std::vector<DrawElement> renderElements;
	renderElements.reserve(elements.size());

	for (size_t i = 0; i < elements.size(); ++i)
	{
		if (auto element_s = std::get_if<SlateDrawElement>(&elements[i]))
		{
			auto& element_d = renderElements.emplace_back();
			if (element_s->Transform.HasRenderTransform())
			{
				const SlateRenderTransform& renderTransform = element_s->Transform.GetAccumulatedRenderTransform();
				element_d.M = renderTransform.GetMatrix();
				element_d.AbsolutePosition = renderTransform.GetTranslation();
				element_d.AbsoluteSize = element_s->Transform.GetLocalSize();
				element_d.Depth = (float)element_s->Layer;
				element_d.TexturePosition = Vector2::GetZero();
				element_d.TextureSize = Vector2::GetOneVector();
			}
		}
		//else if (auto element_s = std::get_if<SlateFontElement>(&elements[i]))
		//{
		//	if (element_s->FontFace && element_s->Text.length())
		//	{
		//		if (renderElements.capacity() <= renderElements.size() + element_s->Text.length())
		//		{
		//			// Reserve space for store glyph render informations.
		//			renderElements.reserve(renderElements.capacity() + element_s->Text.length());
		//		}

		//		 TEST IMPLEMENTATION: Streaming glyphs.
		//		element_s->FontFace->SetFontSize(element_s->FontSize);
		//		fontCachingMgr->StreamGlyphs(element_s->FontFace, element_s->Text);
		//		fontCachingMgr->Apply();

		//		 Get font rendering info.
		//		std::vector glyphInfos = fontCachingMgr->QueryGlyphsRenderInfo(element_s->FontFace, element_s->FontSize, element_s->Text);

		//		Vector2 advance;
		//		const SlateRenderTransform& renderTransform = element_s->Transform.GetAccumulatedRenderTransform();
		//		const Vector2& localTranslation = renderTransform.GetTranslation();
		//		const Vector2& localSize = element_s->Transform.GetLocalSize();
		//		const Matrix2x2& localMatrix = renderTransform.GetMatrix();

		//		for (auto& glyph : glyphInfos)
		//		{
		//			auto& element_d = renderElements.emplace_back();
		//			element_d.M = localMatrix;
		//			element_d.AbsolutePosition = localTranslation + advance + Vector2(0, localSize.Y) - glyph.LocalPosition * Vector2(-1.0f, 1.0f);
		//			element_d.AbsoluteSize = glyph.AbsoluteSize;
		//			element_d.Depth = (float)element_s->Layer;
		//			element_d.TexturePosition = (glyph.AbsolutePosition + Vector2(0.5f, 0.0f)) * glyph.AbsoluteToLocalScale;
		//			element_d.TextureSize = (glyph.AbsoluteSize + Vector2(-0.5f, 1.0f)) * glyph.AbsoluteToLocalScale;

		//			advance += glyph.LocalAdvance;
		//		}
		//	}
		//}
	}

	return renderElements;
}

void SSlateShader::RenderElements(IRHIDeviceContext* deviceContext, const Vector2& screenSize, SSlateWindowElementList* elements)
{
	//SFontCachingManager* fontCachingMgr = GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetFontCachingManager();
	auto elements_span = elements->GetSpan();

	// Caching max elements.
	size_t maxDescriptors = 0;
	for (auto& element : elements_span)
	{
		if (auto element_s = std::get_if<SlateDrawElement>(&element))
		{
			if (element_s->Brush.ImageSource)
			{
				maxDescriptors += 1;
			}
		}
		//else if (auto element_s = std::get_if<SlateFontElement>(&element))
		//{
		//	if (element_s->FontFace)
		//	{
		//		maxDescriptors += 1;
		//	}
		//}
	}
	
	//uint64 gpuAddr = elements->ApplyAndCreateBuffer(deviceContext, this);
	//deviceContext->SetGraphicsRoot32BitConstants(0, 2, &screenSize, 0);

	//_shaderDescriptorView->SetMaxDescriptorCount(maxDescriptors);
	//_shaderDescriptorView->ResetBindings();
	//deviceContext->SetShaderDescriptorViews(_shaderDescriptorView, nullptr);

	//for (auto& element : elements_span)
	//{
	//	if (auto element_s = std::get_if<SlateDrawElement>(&element))
	//	{
	//		if (element_s->Brush.ImageSource)
	//		{
	//			const int32 RenderMode = (int32)ESlateRenderMode::ImageSource;

	//			deviceContext->SetGraphicsRootShaderResourceView(1, gpuAddr);
	//			deviceContext->SetGraphicsRootShaderResourceView(2, element_s->Brush.ImageSource);
	//			deviceContext->SetGraphicsRoot32BitConstants(3, 1, &RenderMode, 0);
	//			deviceContext->DrawInstanced(4, 1);

	//			gpuAddr += sizeof(DrawElement);
	//		}
	//	}
	//	else if (auto element_s = std::get_if<SlateFontElement>(&element))
	//	{
	//		if (element_s->FontFace)
	//		{
	//			SRHIShaderResourceView* imageSource = fontCachingMgr->GetFontFaceRenderingView(element_s->FontFace);
	//			const int32 RenderMode = (int32)ESlateRenderMode::Glyph;

	//			deviceContext->SetGraphicsRootShaderResourceView(1, gpuAddr);
	//			deviceContext->SetGraphicsRootShaderResourceView(2, imageSource);
	//			deviceContext->SetGraphicsRoot32BitConstants(3, 1, &RenderMode, 0);
	//			deviceContext->DrawInstanced(4, (uint32)element_s->Text.length());

	//			gpuAddr += sizeof(DrawElement) * element_s->Text.length();
	//		}
	//	}
	//}
}

std::span<uint8 const> SSlateShader::CompileVS()
{
	return pSlateShaderVS;
}

std::span<uint8 const> SSlateShader::CompilePS()
{
	return pSlateShaderPS;
}