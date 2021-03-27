// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Assets/AssetManager.h"

#include "Engine.h"
#include "Logging/LogVerbosity.h"
#include "Logging/LogCategoryBase.h"
#include "SceneRendering/StaticMesh.h"
#include "Materials/Material.h"
#include "DirectX/DirectXGeometry.h"

using namespace std;

AssetManager::AssetManager(Engine* engine) : Super()
	, engine(engine)
{
	InitEngineAssets();
}

AssetManager::~AssetManager()
{

}

void AssetManager::Import(TRefPtr<String> key, TRefPtr<StaticMesh> imported)
{
	staticMeshes.insert(make_pair(key, imported));
}

StaticMesh* AssetManager::LoadStaticMesh(TRefPtr<String> key) const
{
	auto it = staticMeshes.find(key);
	if (it == staticMeshes.end())
	{
		return nullptr;
	}

	return it->second.Get();
}

void AssetManager::Import(TRefPtr<String> key, TRefPtr<Material> imported)
{
	materials.insert(make_pair(key, imported));
}

Material* AssetManager::LoadMaterial(TRefPtr<String> key) const
{
	auto it = materials.find(key);
	if (it == materials.end())
	{
		return nullptr;
	}

	return it->second.Get();
}

void AssetManager::InitEngineAssets()
{	
	Import(L"Engine/Materials/Default", NewObject<Material>(engine->GetDeviceBundle()));

	auto RegisterSingleSubsetGeometry = [&](wstring_view importPath, const vector<Vertex>& vc, const vector<uint32>& ic)
	{
		StaticMeshSubsetInfo subset = { };
		subset.VertexStart = 0;
		subset.VertexCount = (uint32)vc.size();
		subset.IndexStart = 0;
		subset.IndexCount = (uint32)ic.size();
		subset.Material = LoadMaterial(L"Engine/Materials/Default");
	
		StaticMeshGeometryData data;
		data.VertexBuffer = vc;
		data.IndexBuffer = ic;
		data.Subsets.emplace_back(subset);
	
		Import(importPath.data(), NewObject<StaticMesh>(engine, data));
	};

	auto RegisterDirectXGeometry = [&](auto computeGeometry, wstring_view path)
	{
		vector<Vertex> vertexCollection;
		vector<uint32> indexCollection;
		computeGeometry(vertexCollection, indexCollection);
		RegisterSingleSubsetGeometry(path, vertexCollection, indexCollection);
	};

	{
		using namespace placeholders;
		RegisterDirectXGeometry(bind(&DirectXGeometry::ComputeBox, _1, _2, 1.0f, false, false), L"Engine/StaticMesh/Box");
		RegisterDirectXGeometry(bind(&DirectXGeometry::ComputeSphere, _1, _2, 1.0f, 16, false, false), L"Engine/StaticMesh/Sphere");
		RegisterDirectXGeometry(bind(&DirectXGeometry::ComputeGeoSphere, _1, _2, 1.0f, 4, false), L"Engine/StaticMesh/GeoSphere");
		RegisterDirectXGeometry(bind(&DirectXGeometry::ComputeCylinder, _1, _2, 1.0f, 1.0f, 16, false), L"Engine/StaticMesh/Cylinder");
		RegisterDirectXGeometry(bind(&DirectXGeometry::ComputeCone, _1, _2, 1.0f, 1.0f, 16, false), L"Engine/StaticMesh/Cone");
		RegisterDirectXGeometry(bind(&DirectXGeometry::ComputeTetrahedron, _1, _2, 1.0f, false), L"Engine/StaticMesh/Tetrahedron");
		RegisterDirectXGeometry(bind(&DirectXGeometry::ComputeOctahedron, _1, _2, 1.0f, false), L"Engine/StaticMesh/Octahedron");
		RegisterDirectXGeometry(bind(&DirectXGeometry::ComputeDodecahedron, _1, _2, 1.0f, false), L"Engine/StaticMesh/Dodecahedron");
		RegisterDirectXGeometry(bind(&DirectXGeometry::ComputeIcosahedron, _1, _2, 1.0f, false), L"Engine/StaticMesh/Icosahedron");
		RegisterDirectXGeometry(bind(&DirectXGeometry::ComputeTeapot, _1, _2, 1.0f, 16, false), L"Engine/StaticMesh/Teapot");
	}
}