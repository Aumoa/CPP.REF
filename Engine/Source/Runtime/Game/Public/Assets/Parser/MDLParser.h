// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "ParserBase.h"

class GAME_API MDLParser : public ParserBase
{
	CLASS_BODY(MDLParser)

public:
	MDLParser(GameEngine* engine, RHIVertexFactory* vfactory);
	~MDLParser() override;

	virtual bool TryParse(const std::filesystem::path& importPath) override;

	virtual bool IsStaticMesh() const override;
	virtual StaticMesh* GetStaticMesh() const override;
};