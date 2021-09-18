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
		node_it = _nodes.emplace(faceName, NewObject<SFontFaceCachingNode>(_device)).first;
	}

	SFontFaceCachingNode* node = node_it->second;
	node->StreamGlyphs(face, glyphs);
}

void SFontCachingManager::Apply()
{
	for (auto& node_it : _nodes)
	{
		node_it.second->Apply();
	}
}