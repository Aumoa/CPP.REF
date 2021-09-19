// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "FreeType/FontCachingManager.h"
#include "FreeType/FontFace.h"
#include "FreeType/FontFaceCachingNode.h"

SFontCachingManager::SFontCachingManager(SRHIDevice* device) : Super()
	, _device(device)
{
}

SFontCachingManager::~SFontCachingManager()
{
}

void SFontCachingManager::StreamGlyphs(SFontFace* face, std::wstring_view glyphs)
{
	check(face);

	std::wstring faceName = face->GetName();
	auto node_it = _nodes.find(faceName);
	if (node_it == _nodes.end())
	{
		node_it = _nodes.emplace(faceName, NewObject<SFontFaceCachingNode>(face, _device)).first;
	}

	SFontFaceCachingNode* node = node_it->second;
	node->StreamGlyphs(glyphs);
}

void SFontCachingManager::Apply()
{
	for (auto& node_it : _nodes)
	{
		node_it.second->Apply();
	}
}

SRHIShaderResourceView* SFontCachingManager::GetFontFaceRenderingView(SFontFace* face) const
{
	check(face);
	std::wstring faceName = face->GetName();
	auto node_it = _nodes.find(faceName);
	if (node_it == _nodes.end())
	{
		return nullptr;
	}

	return node_it->second->GetRenderingView();
}

std::vector<GlyphRenderInfo> SFontCachingManager::QueryGlyphsRenderInfo(SFontFace* face, int32 fontSize, std::wstring_view text) const
{
	check(face);
	std::wstring faceName = face->GetName();
	auto node_it = _nodes.find(faceName);
	if (node_it == _nodes.end())
	{
		return {};
	}

	SFontFaceCachingNode* node = node_it->second;
	return node->QueryGlyphsRenderInfo(fontSize, text);
}