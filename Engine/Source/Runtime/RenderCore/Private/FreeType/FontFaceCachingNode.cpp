// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "FreeType/FontFaceCachingNode.h"
#include "FreeType/FontFace.h"
#include "RHI/RHIDynamicTexture2D.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHICommandQueue.h"
#include "RHI/RHIShaderResourceView.h"

SFontFaceCachingNode::SFontFaceCachingNode(SRHIDevice* device) : Super()
	, _device(device)
{
	_shaderResourceView = NewObject<SRHIShaderResourceView>(device, 1);
}

SFontFaceCachingNode::~SFontFaceCachingNode()
{
}

void SFontFaceCachingNode::StreamGlyphs(SFontFace* face, std::wstring_view glyphs)
{
	int32 requiredMaxWidth = _requiredMaxWidth;
	int32 requiredMaxHeight = _requiredMaxHeight;

	for (size_t i = 0; i < glyphs.length(); ++i)
	{
		wchar_t character = glyphs[i];
		int32 fontSize = face->GetFontSize();
		auto collection_it = _glyphCollectionsView.find(fontSize);
		if (collection_it == _glyphCollectionsView.end())
		{
			// Caching new collection.
			size_t index = _glyphCollections.size();
			auto& collection = _glyphCollections.emplace_back();
			collection_it = _glyphCollectionsView.emplace(fontSize, index).first;
			collection.RequiredMaxHeight = face->GetMaxGlyphRenderHeight();
			requiredMaxHeight += collection.RequiredMaxHeight;

			_bNeedApply = true;
		}

		auto& collection = _glyphCollections[collection_it->second];
		auto& glyphsView = collection.GlyphsView;

		if (auto it = glyphsView.find(character); it != glyphsView.end())
		{
			collection.Glyphs[it->second].Hit += 1;
			continue;
		}

		// Caching new glyph information.
		if (!face->LoadGlyph(character))
		{
			continue;
		}

		size_t index = collection.Glyphs.size();
		auto& glyph = collection.Glyphs.emplace_back();
		collection.GlyphsView.emplace(character, index);
		_bNeedApply = true;

		glyph.Character = character;
		glyph.GlyphIndex = face->GetGlyphIndex();
		glyph.PixelSize = face->GetGlyphPixelSize();
		glyph.Bitmap.resize((size_t)glyph.PixelSize.X * glyph.PixelSize.Y);
		glyph.LocalPosition = face->GetLocalPosition();
		glyph.LocalAdvance = face->GetAdvance();

		// Copy glyph pixels.
		ensure(face->CopyGlyphPixels(glyph.Bitmap.data(), glyph.PixelSize.X, 0, 0));

		// Update required sizes.
		collection.RequiredMaxWidth += glyph.PixelSize.X;
		requiredMaxWidth = std::max(requiredMaxWidth, collection.RequiredMaxWidth);
	}

	_requiredMaxWidth = requiredMaxWidth;
	_requiredMaxHeight = requiredMaxHeight;
}

#define ShouldBeReload(Body) (!(Body).bLoad || bForceReload)

void SFontFaceCachingNode::Apply()
{
	if (!_bNeedApply)
	{
		return;
	}

	const bool bForceReload = ReallocateBufferIfRequired();
	
	int32 locationY = 0;
	for (SizedGlyphCollection& collection : _glyphCollections)
	{
		// Load collection if possible.
		if (ShouldBeReload(collection))
		{
			collection.LocationY = locationY;
			collection.bLoad = true;
		}

		int32 locationX = 0;
		for (Glyph& glyph : collection.Glyphs)
		{
			if (ShouldBeReload(glyph))
			{
				const int32 pixelStride = 1; // 0~255

				// Update pixels.
				glyph.LocationX = locationX;
				_glyphBuffer->UpdatePixels(glyph.Bitmap.data(), pixelStride, glyph.PixelSize, Vector2N(glyph.LocationX, collection.LocationY));

				glyph.bLoad = true;
			}

			locationX += glyph.PixelSize.X;
		}

		locationY += collection.RequiredMaxHeight;
	}

	_glyphBuffer->Apply();
	_bNeedApply = false;
}

#undef ShouldBeReload

SRHIShaderResourceView* SFontFaceCachingNode::GetRenderingView() const
{
	return _shaderResourceView;
}

std::vector<GlyphRenderInfo> SFontFaceCachingNode::QueryGlyphsRenderInfo(SFontFace* face, int32 fontSize, std::wstring_view text) const
{
	if (_glyphBuffer == nullptr)
	{
		return {};
	}

	auto sizedGlyphsCollection_it = _glyphCollectionsView.find(fontSize);
	if (sizedGlyphsCollection_it == _glyphCollectionsView.end())
	{
		return {};
	}

	const SizedGlyphCollection& collection = _glyphCollections[sizedGlyphsCollection_it->second];
	if (!collection.bLoad)
	{
		return {};
	}

	const Vector2 pixelSize = _glyphBuffer->GetPixelSize().Cast<float>();

	std::vector<GlyphRenderInfo> glyphs;
	glyphs.reserve(text.length());

	std::vector<const Glyph*> loadedGlyphs;
	loadedGlyphs.reserve(text.length());

	// Load glyphs.
	for (auto& ch : text)
	{
		auto glyph_view_it = collection.GlyphsView.find(ch);
		if (glyph_view_it == collection.GlyphsView.end())
		{
			continue;
		}

		auto& glyph = collection.Glyphs[glyph_view_it->second];
		if (!glyph.bLoad)
		{
			continue;
		}

		loadedGlyphs.emplace_back(&glyph);
	}

	// Make render info.
	const auto absoluteToLocalScale = 1.0f / pixelSize;
	for (size_t i = 0; i < loadedGlyphs.size(); ++i)
	{
		auto& glyph = loadedGlyphs[i];

		GlyphRenderInfo& renderInfo = glyphs.emplace_back();
		renderInfo.LocalPosition = glyph->LocalPosition;
		renderInfo.LocalAdvance = glyph->LocalAdvance;

		if (i < loadedGlyphs.size() - 1)
		{
			renderInfo.LocalAdvance += face->GetKerning(glyph->GlyphIndex, loadedGlyphs[i + 1]->GlyphIndex);
		}

		renderInfo.AbsolutePosition = Vector2((float)glyph->LocationX, (float)collection.LocationY);
		renderInfo.AbsoluteSize = glyph->PixelSize.Cast<float>();
		renderInfo.AbsoluteToLocalScale = absoluteToLocalScale;
	}

	return glyphs;
}

inline static int32 Align256(const int32& value)
{
	return (value + 255) & ~255;
}

bool SFontFaceCachingNode::ReallocateBufferIfRequired()
{
	Vector2N pixelSize;
	if (_glyphBuffer)
	{
		pixelSize = _glyphBuffer->GetPixelSize();
	}

	if (_requiredMaxWidth <= pixelSize.X && _requiredMaxHeight <= pixelSize.Y)
	{
		// We need not to reallocate.
		return false;
	}

	if (_glyphBuffer)
	{
		SRHICommandQueue* queue = _device->GetPrimaryQueue();
		queue->AddGarbageObject(queue->GetLastSignal(), _glyphBuffer);
		_glyphBuffer = nullptr;
	}

	// Create new texture buffer.
	SRHIDynamicTexture2D* texture = _device->CreateDynamicTexture2D(
		ERHIResourceStates::PixelShaderResource,
		ERHIPixelFormat::R8_UNORM,
		Align256(_requiredMaxWidth),
		Align256(_requiredMaxHeight), std::nullopt);

	texture->SetOuter(this);
	_glyphBuffer = texture;
	_shaderResourceView->CreateShaderResourceView(texture, 0);

	return true;
}

SRHIShaderResourceView* SFontFaceCachingNode::GetDebugTexture() const
{
	return _shaderResourceView;
}