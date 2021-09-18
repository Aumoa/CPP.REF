// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Shaders/SlateShader/SlateShader.h"
#include "Materials/Material.h"
#include "SlateMinimal.h"
#include "GameEngine.h"
#include "Draw/SlateWindowElementList.h"
#include "EngineSubsystems/GameRenderSystem.h"

#ifndef BYTE
#define BYTE uint8
#endif

#include "SlateShaderVS.hlsl.h"
#include "SlateShaderPS.hlsl.h"

SSlateShader::SSlateShader(SRHIDevice* device) : Super(device)
{
	class SlateShaderMaterial : public SMaterial
	{
	public:
		using Super = SMaterial;

	public:
		SlateShaderMaterial(SSlateShader* shader) : Super(shader)
		{
		}

		virtual int32 GetRootParameterMappingIndex(std::wstring_view parameterName) const override
		{
			if (parameterName == L"Color")
			{
				return 1;
			}
			else
			{
				return Super::GetRootParameterMappingIndex(parameterName);
			}
		}
	};

	_material = NewObject<SlateShaderMaterial>(this);
	_shaderDescriptorView = NewObject<SRHIShaderDescriptorView>(GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetRHIDevice());
}

void SSlateShader::Compile(SRHIVertexFactory* vertexDeclaration)
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

	Super::Compile(vertexDeclaration);
}

std::vector<RHIShaderParameterElement> SSlateShader::GetShaderParameterDeclaration() const
{
	std::vector<RHIShaderParameterElement> elements;
	elements.reserve(4);

	// [0] SlateConstants
	elements.emplace_back() =
	{
		.Type = ERHIShaderParameterType::ScalarParameterConstants,
		.ScalarConstantsParameter =
		{
			.ShaderRegister = 0,
			.Num32Bits = 2,
		}
	};

	// [1] SlateElements
	elements.emplace_back() =
	{
		.Type = ERHIShaderParameterType::StructuredBuffer,
		.StructuredBuffer =
		{
			.ShaderRegister = 0,
		}
	};

	// [2] ImageSource
	elements.emplace_back() =
	{
		.Type = ERHIShaderParameterType::DescriptorTable,
		.DescriptorTable =
		{
			.NumDescriptorRanges = (uint32)_imageSourceRanges.size(),
			.pDescriptorRanges = _imageSourceRanges.data()
		}
	};

	// [3] RenderMode
	elements.emplace_back() =
	{
		.Type = ERHIShaderParameterType::ScalarParameterConstants,
		.ScalarConstantsParameter =
		{
			.ShaderRegister = 1,
			.Num32Bits = 1
		}
	};

	// [4] FontFaceBuffer
	elements.emplace_back() =
	{
		.Type = ERHIShaderParameterType::DescriptorTable,
		.DescriptorTable =
		{
			.NumDescriptorRanges = (uint32)_fontFaceBufferRanges.size(),
			.pDescriptorRanges = _fontFaceBufferRanges.data()
		}
	};

	return elements;
}

SMaterial* SSlateShader::GetDefaultMaterial() const
{
	return _material;
}

auto SSlateShader::MakeElements(const std::vector<SSlateWindowElementList::GenericSlateElement>& elements) const -> std::vector<DrawElement>
{
	std::vector<DrawElement> renderElements;
	renderElements.reserve(elements.size());

	for (size_t i = 0; i < elements.size(); ++i)
	{
		auto& element_d = renderElements.emplace_back();

		if (auto element_s = std::get_if<SlateDrawElement>(&elements[i]))
		{
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
		else if (auto element_s = std::get_if<SlateFontElement>(&elements[i]))
		{
		}
	}

	return renderElements;
}

void SSlateShader::RenderElements(SRHIDeviceContext* deviceContext, const Vector2& screenSize, SSlateWindowElementList* elements)
{
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
		else if (auto element_s = std::get_if<SlateFontElement>(&element))
		{
			if (element_s->CachingNode)
			{
				maxDescriptors += 1;
			}
		}
	}
	
	uint64 gpuAddr = elements->ApplyAndCreateBuffer(deviceContext, this);
	deviceContext->SetGraphicsRoot32BitConstants(0, 2, &screenSize, 0);

	_shaderDescriptorView->SetMaxDescriptorCount(maxDescriptors);
	_shaderDescriptorView->ResetBindings();
	deviceContext->SetShaderDescriptorViews(_shaderDescriptorView, nullptr);

	for (auto& element : elements_span)
	{
		if (auto element_s = std::get_if<SlateDrawElement>(&element))
		{
			if (element_s->Brush.ImageSource)
			{
				const int32 RenderMode = (int32)ESlateRenderMode::ImageSource;

				deviceContext->SetGraphicsRootShaderResourceView(1, gpuAddr);
				deviceContext->SetGraphicsRootShaderResourceView(2, element_s->Brush.ImageSource);
				deviceContext->SetGraphicsRoot32BitConstants(3, 1, &RenderMode, 0);
				deviceContext->DrawInstanced(4, 1);
			}

			gpuAddr += sizeof(DrawElement);
		}
		else if (auto element_s = std::get_if<SlateFontElement>(&element))
		{
		}
	}
}

std::span<uint8 const> SSlateShader::CompileVS()
{
	return pSlateShaderVS;
}

std::span<uint8 const> SSlateShader::CompilePS()
{
	return pSlateShaderPS;
}