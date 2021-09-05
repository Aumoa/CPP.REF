// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "ParserBase.h"
#include <memory>

class GAME_API AssimpParser : public ParserBase
{
	GENERATED_BODY(AssimpParser)

private:
	class Impl;

	std::unique_ptr<Impl> _impl;
	std::filesystem::path _path;
	std::filesystem::path _name;
	std::filesystem::path _parent;
	StaticMesh* _mesh = nullptr;

public:
	AssimpParser(GameEngine* engine, RHIVertexFactory* vfactory);
	~AssimpParser() override;

	virtual bool TryParse(const std::filesystem::path& importPath) override;

	virtual bool IsStaticMesh() const override;
	virtual StaticMesh* GetStaticMesh() const override;

private:
	//bool ProcessMaterials();
	bool ProcessStaticMeshSubsets();
	//TComPtr<ID3D12Resource> LoadImageTexture(TRefPtr<String> filepath);
};