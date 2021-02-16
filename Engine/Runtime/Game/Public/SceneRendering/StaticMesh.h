// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Mesh.h"

#include "MeshBatch.h"
#include "RHI/RHIMeshDrawCommand.h"
#include "RHI/RHIVertex.h"
#include "Materials/Material.h"

class StaticMeshBatch;
interface IRHIResource;

class GAME_API StaticMeshBatch : public MeshBatch
{
public:
	using Super = MeshBatch;
	using This = StaticMeshBatch;

private:
	RHIMeshDrawCommand drawCommand;

public:
	StaticMeshBatch();
	~StaticMeshBatch() override;

	const RHIMeshDrawCommand* GetDrawCommand() const override;

	void SetMeshDrawCommand(const RHIMeshDrawCommand& inDrawCommand);
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
	AxisAlignedCube boundingBox;

	TRefPtr<Material> material;

public:
	StaticMesh();
	~StaticMesh();

	MeshBatch* GetMeshBatch() const;
	
	vs_property_get_auto(MaterialInterface*, DefaultMaterial, material.Get());
	vs_property_get_auto(AxisAlignedCube, BoundingBox, boundingBox);

	static TRefPtr<StaticMesh> CreateStaticMesh(std::span<RHIVertex> vertices, std::span<uint32> indices, TRefPtr<Material> defaultMaterial);
	static TRefPtr<StaticMesh> CreateStaticMesh(std::span<RHIVertex> vertices, std::span<uint32> indices, TRefPtr<Material> defaultMaterial, const AxisAlignedCube& inBoundingBox);

private:
	static AxisAlignedCube ComputeBoundingBox(std::span<RHIVertex> vertices);
};