// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Mesh.h"

#include "Vertex.h"
#include "DirectX/DirectXMinimal.h"
#include "Materials/Material.h"

class DirectXDeviceBundle;

struct StaticMeshSubsetInfo
{
	uint32 VertexStart;
	uint32 VertexCount;
	uint32 IndexStart;
	uint32 IndexCount;
	Material* Material;
};

struct StaticMeshGeometryData
{
	std::vector<Vertex> VertexBuffer;
	std::vector<uint32> IndexBuffer;
	std::vector<StaticMeshSubsetInfo> Subsets;

	void Clear(bool bShrinkToFit = false);
	void AddSubset(std::span<Vertex const> inVertices, std::span<uint32 const> inIndices, Material* inMaterial);
};

class GAME_API StaticMesh : public Mesh
{
public:
	using Super = Mesh;

private:
	TComPtr<ID3D12Resource> vertexBuffer;
	TComPtr<ID3D12Resource> indexBuffer;
	TComPtr<ID3D12Resource> accelerationStructure;
	AxisAlignedCube boundingBox;
	std::vector<StaticMeshSubsetInfo> subsets;
	std::vector<TRefPtr<Material>> materialCapture;

public:
	StaticMesh(Engine* engine, const StaticMeshGeometryData& inGeometryData);
	~StaticMesh();
	
	vs_property_get_auto(AxisAlignedCube, BoundingBox, boundingBox);
	vs_property_get_auto(ID3D12Resource*, RaytracingAccelerationStructureBuffer, accelerationStructure.Get());
	vs_property_get_auto(ID3D12Resource*, VertexBuffer, vertexBuffer.Get());
	vs_property_get_auto(ID3D12Resource*, IndexBuffer, indexBuffer.Get());
	vs_property_get_auto(std::vector<StaticMeshSubsetInfo>, Subsets, subsets);

private:
	static AxisAlignedCube ComputeBoundingBox(std::span<Vertex const> vertices);
};