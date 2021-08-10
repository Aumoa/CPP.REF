// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/Parser/MDLParser.h"

MDLParser::MDLParser(GameEngine* engine, RHIVertexFactory* vfactory) : Super(engine, vfactory)
{
}

MDLParser::~MDLParser()
{
}

bool MDLParser::TryParse(const std::filesystem::path& importPath)
{
	return false;
}

bool MDLParser::IsStaticMesh() const
{
	return false;
}

StaticMesh* MDLParser::GetStaticMesh() const
{
	return nullptr;
}