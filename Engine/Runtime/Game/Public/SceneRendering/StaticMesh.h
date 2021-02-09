// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Mesh.h"

#include "MeshBatch.h"
#include "RHi/RHIMeshDrawCommand.h"
#include "RHI/RHIVertex.h"

class Material;
interface IRHIResource;

class GAME_API StaticMeshBatch : public MeshBatch
{
public:
	using Super = MeshBatch;
	using This = StaticMeshBatch;

private:
	RHIMeshDrawCommand drawCommand;

public:
	StaticMeshBatch(const RHIMeshDrawCommand& drawCommand);
	~StaticMeshBatch() override;

	const RHIMeshDrawCommand* GetDrawCommand() const override;
};

class GAME_API StaticMesh : public Mesh
{
public:
	using Super = Mesh;
	using This = StaticMesh;

private:
	TRefPtr<IRHIResource> vertexBuffer;
	TRefPtr<IRHIResource> indexBuffer;
	TRefPtr<StaticMeshBatch> meshBatch;

	TRefPtr<Material> material;

public:
	StaticMesh();
	~StaticMesh();

	MeshBatch* GetMeshBatch() const;
	
	vs_property_get_auto(Material*, DefaultMaterial, material.Get());

	static TRefPtr<StaticMesh> CreateStaticMesh(std::span<RHIVertex> vertices, std::span<uint32> indices, TRefPtr<Material> defaultMaterial);
};