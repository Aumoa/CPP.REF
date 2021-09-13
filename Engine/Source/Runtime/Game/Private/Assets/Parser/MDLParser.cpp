// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/Parser/MDLParser.h"

SMDLParser::SMDLParser(SGameEngine* engine, SRHIVertexFactory* vfactory) : Super(engine, vfactory)
{
}

SMDLParser::~SMDLParser()
{
}

bool SMDLParser::TryParse(const std::filesystem::path& importPath)
{
	return false;
}

bool SMDLParser::IsStaticMesh() const
{
	return false;
}

SStaticMesh* SMDLParser::GetStaticMesh() const
{
	return nullptr;
}