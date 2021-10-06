// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SGameEngine;
class SStaticMesh;
class SVertexFactory;

class GAME_API SParserBase : implements SObject
{
	GENERATED_BODY(SParserBase)

private:
	SGameEngine* _engine = nullptr;
	SVertexFactory* _factory = nullptr;

public:
	SParserBase(SGameEngine* engine, SVertexFactory* vfactory) : Super()
		, _engine(engine)
		, _factory(vfactory)
	{
	}

	virtual bool TryParse(const std::filesystem::path& importPath) = 0;

	virtual bool IsStaticMesh() = 0;
	virtual SStaticMesh* GetStaticMesh() = 0;

	inline SGameEngine* GetEngine() { return _engine; }
	inline SVertexFactory* GetVertexFactory() { return _factory; }
};