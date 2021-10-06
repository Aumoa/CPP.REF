// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ParserBase.h"

class GAME_API SMDLParser : public SParserBase
{
	GENERATED_BODY(SMDLParser)

public:
	SMDLParser(SGameEngine* engine, SVertexFactory* vfactory);
	~SMDLParser() override;

	virtual bool TryParse(const std::filesystem::path& importPath) override;

	virtual bool IsStaticMesh() override;
	virtual SStaticMesh* GetStaticMesh() override;
};