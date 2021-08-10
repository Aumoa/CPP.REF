// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"

class GameEngine;
class StaticMesh;
class RHIVertexFactory;

class GAME_API ParserBase : virtual public Object
{
public:
	using Super = Object;

private:
	GameEngine* _engine = nullptr;
	RHIVertexFactory* _factory = nullptr;

public:
	ParserBase(GameEngine* engine, RHIVertexFactory* vfactory) : Super()
		, _engine(engine)
		, _factory(vfactory)
	{
	}

	virtual bool TryParse(const std::filesystem::path& importPath) = 0;

	virtual bool IsStaticMesh() const = 0;
	virtual StaticMesh* GetStaticMesh() const = 0;

	inline GameEngine* GetEngine() const { return _engine; }
	inline RHIVertexFactory* GetVertexFactory() const { return _factory; }
};