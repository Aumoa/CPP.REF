// Copyright 2020 Aumoa.lib. All right reserved.

#include "Engine.h"

#include "DirectXGeometry.h"
#include "Assets/AssetManager.h"
#include "SceneRendering/StaticMesh.h"
#include "Materials/Material.h"
#include "RHI/IRHIMaterialBundle.h"

using namespace std;

inline void LoadEngineDefaultMaterials(Engine* engine)
{
	auto bundle = engine->MaterialBundle;
	auto mgr = engine->GetAssetManager();

	{
		TRefPtr<MaterialInterface> material = bundle->CreateMaterial();
		material->Ambient = 0.2f;
		material->Diffuse = 0.5f;
		material->Specular = 0.8f;
		material->SpecExp = 32.0f;
		mgr->Import(L"Engine/Materials/Default", move(material).As<Material>());
	}

	{
		TRefPtr<MaterialInterface> material = bundle->CreateMaterial();
		material->Ambient = 0.2f;
		material->Diffuse = 0.5f;
		material->Specular = 0.8f;
		material->SpecExp = 32.0f;
		mgr->Import(L"Engine/Materials/Default1", move(material).As<Material>());
	}

	{
		TRefPtr<MaterialInterface> material = bundle->CreateMaterial();
		material->Ambient = 0.2f;
		material->Diffuse = 0.5f;
		material->Specular = 0.8f;
		material->SpecExp = 32.0f;
		mgr->Import(L"Engine/Materials/Default2", move(material).As<Material>());
	}

	{
		TRefPtr<MaterialInterface> material = bundle->CreateMaterial();
		material->Ambient = 0.2f;
		material->Diffuse = 0.5f;
		material->Specular = 0.8f;
		material->SpecExp = 32.0f;
		mgr->Import(L"Engine/Materials/Default3", move(material).As<Material>());
	}

	{
		TRefPtr<MaterialInterface> material = bundle->CreateMaterial();
		material->Ambient = 0.2f;
		material->Diffuse = 0.5f;
		material->Specular = 0.8f;
		material->SpecExp = 32.0f;
		mgr->Import(L"Engine/Materials/Default4", move(material).As<Material>());
	}

	{
		TRefPtr<MaterialInterface> material = bundle->CreateMaterial();
		material->Ambient = 0.2f;
		material->Diffuse = 0.5f;
		material->Specular = 0.8f;
		material->SpecExp = 32.0f;
		mgr->Import(L"Engine/Materials/Default5", move(material).As<Material>());
	}

	{
		TRefPtr<MaterialInterface> material = bundle->CreateMaterial();
		material->Ambient = 0.2f;
		material->Diffuse = 0.5f;
		material->Specular = 0.8f;
		material->SpecExp = 32.0f;
		mgr->Import(L"Engine/Materials/Default6", move(material).As<Material>());
	}

	{
		TRefPtr<MaterialInterface> material = bundle->CreateMaterial();
		material->Ambient = 0.2f;
		material->Diffuse = 0.5f;
		material->Specular = 0.8f;
		material->SpecExp = 32.0f;
		mgr->Import(L"Engine/Materials/Default7", move(material).As<Material>());
	}

	{
		TRefPtr<MaterialInterface> material = bundle->CreateMaterial();
		material->Ambient = 0.2f;
		material->Diffuse = 0.5f;
		material->Specular = 0.8f;
		material->SpecExp = 32.0f;
		mgr->Import(L"Engine/Materials/Default8", move(material).As<Material>());
	}

	{
		TRefPtr<MaterialInterface> material = bundle->CreateMaterial();
		material->Ambient = 0.2f;
		material->Diffuse = 0.5f;
		material->Specular = 0.8f;
		material->SpecExp = 32.0f;
		mgr->Import(L"Engine/Materials/Default9", move(material).As<Material>());
	}
}

inline void LoadEngineDefaultStaticMeshes(Engine* engine)
{
	auto mgr = engine->GetAssetManager();
	auto mat = mgr->LoadMaterial(L"Engine/Materials/Default");
	auto mat2 = mgr->LoadMaterial(L"Engine/Materials/Default9");

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeBox(vertexCollection, indexCollection, 1.0f, false, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection, mat);
		mgr->Import(L"Engine/StaticMesh/Box", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeSphere(vertexCollection, indexCollection, 1.0f, 16, false, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection, mat);
		mgr->Import(L"Engine/StaticMesh/Sphere", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeGeoSphere(vertexCollection, indexCollection, 1.0f, 4, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection, mat);
		mgr->Import(L"Engine/StaticMesh/GeoSphere", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeCylinder(vertexCollection, indexCollection, 1.0f, 1.0f, 16, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection, mat);
		mgr->Import(L"Engine/StaticMesh/Cylinder", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeCone(vertexCollection, indexCollection, 1.0f, 1.0f, 16, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection, mat);
		mgr->Import(L"Engine/StaticMesh/Cone", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeTetrahedron(vertexCollection, indexCollection, 1.0f, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection, mat2);
		mgr->Import(L"Engine/StaticMesh/Tetrahedron", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeOctahedron(vertexCollection, indexCollection, 1.0f, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection, mat2);
		mgr->Import(L"Engine/StaticMesh/Octahedron", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeDodecahedron(vertexCollection, indexCollection, 1.0f, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection, mat2);
		mgr->Import(L"Engine/StaticMesh/Dodecahedron", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeIcosahedron(vertexCollection, indexCollection, 1.0f, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection, mat2);
		mgr->Import(L"Engine/StaticMesh/Icosahedron", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeTeapot(vertexCollection, indexCollection, 1.0f, 16, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection, mat2);
		mgr->Import(L"Engine/StaticMesh/Teapot", mesh);
	}
}

void Engine::LoadEngineDefaultAssets()
{
	LoadEngineDefaultMaterials(this);
	LoadEngineDefaultStaticMeshes(this);
}