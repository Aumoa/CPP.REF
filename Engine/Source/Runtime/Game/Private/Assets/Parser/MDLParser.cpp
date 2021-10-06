// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Assets/Parser/MDLParser.h"

SMDLParser::SMDLParser(SGameEngine* engine, SVertexFactory* vfactory) : Super(engine, vfactory)
{
}

SMDLParser::~SMDLParser()
{
}

bool SMDLParser::TryParse(const std::filesystem::path& importPath)
{
	return false;
}

bool SMDLParser::IsStaticMesh()
{
	return false;
}

SStaticMesh* SMDLParser::GetStaticMesh()
{
	return nullptr;
}