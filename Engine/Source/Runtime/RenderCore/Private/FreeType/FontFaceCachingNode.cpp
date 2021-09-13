// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "FontFaceCachingNode.h"
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
	int32 requiredMaxHeight = 0;

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
			return;
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
		ensure(face->GetGlyphPixelSize(&glyph.PixelSizeX, &glyph.PixelSizeY));
		glyph.Bitmap.resize((size_t)glyph.PixelSizeX * glyph.PixelSizeY);

		// Copy glyph pixels.
		ensure(face->CopyGlyphPixels(glyph.Bitmap.data(), glyph.PixelSizeX, 0, 0));

		// Update required sizes.
		collection.RequiredMaxWidth += glyph.PixelSizeX;
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
				_glyphBuffer->UpdatePixels(glyph.Bitmap.data(), pixelStride, glyph.PixelSizeX, glyph.PixelSizeY, glyph.LocationX, collection.LocationY);

				glyph.bLoad = true;
			}

			locationX += glyph.PixelSizeX;
		}

		locationY += collection.RequiredMaxHeight;
	}

	_glyphBuffer->Apply();
	_bNeedApply = false;
}

#undef ShouldBeReload

inline static int32 Align256(const int32& value)
{
	return (value + 255) & ~255;
}

bool SFontFaceCachingNode::ReallocateBufferIfRequired()
{
	int32 pixelWidth = 0, pixelHeight = 0;
	if (_glyphBuffer)
	{
		_glyphBuffer->GetPixelSize(&pixelWidth, &pixelHeight);
	}

	if (_requiredMaxWidth <= pixelWidth && _requiredMaxHeight <= pixelHeight)
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