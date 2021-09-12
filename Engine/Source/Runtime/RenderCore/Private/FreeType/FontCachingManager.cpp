// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "FreeType/FontCachingManager.h"
#include "FreeType/FontFace.h"
#include "FontFaceCachingNode.h"

FontCachingManager::FontCachingManager(RHIDevice* device) : Super()
	, _device(device)
{
}

FontCachingManager::~FontCachingManager()
{
}

void FontCachingManager::StreamGlyphs(FontFace* face, std::wstring_view glyphs)
{
	check(face);

	std::wstring faceName = face->GetName();
	auto node_it = _nodes.find(faceName);
	if (node_it == _nodes.end())
	{
		node_it = _nodes.emplace(faceName, NewObject<FontFaceCachingNode>(_device)).first;
	}

	FontFaceCachingNode* node = node_it->second;
	node->StreamGlyphs(face, glyphs);
}

void FontCachingManager::Apply()
{
	for (auto& node_it : _nodes)
	{
		node_it.second->Apply();
	}
}

RHIShaderResourceView* FontCachingManager::GetDebugTexture() const
{
	return _nodes.begin()->second->GetDebugTexture();
}