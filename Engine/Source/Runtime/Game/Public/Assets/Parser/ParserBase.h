// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"

class SGameEngine;
class SStaticMesh;
class SRHIVertexFactory;

class GAME_API SParserBase : implements SObject
{
	GENERATED_BODY(SParserBase)

private:
	SGameEngine* _engine = nullptr;
	SRHIVertexFactory* _factory = nullptr;

public:
	SParserBase(SGameEngine* engine, SRHIVertexFactory* vfactory) : Super()
		, _engine(engine)
		, _factory(vfactory)
	{
	}

	virtual bool TryParse(const std::filesystem::path& importPath) = 0;

	virtual bool IsStaticMesh() const = 0;
	virtual SStaticMesh* GetStaticMesh() const = 0;

	inline SGameEngine* GetEngine() const { return _engine; }
	inline SRHIVertexFactory* GetVertexFactory() const { return _factory; }
};