// Copyright 2020 Aumoa.lib. All right reserved.

#include "Engine.h"

#include "DirectXGeometry.h"
#include "Assets/AssetManager.h"
#include "SceneRendering/StaticMesh.h"

using namespace std;

void Engine::LoadEngineDefaultAssets()
{
	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeBox(vertexCollection, indexCollection, 1.0f, false, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection);
		GetAssetManager()->Import(L"Engine/StaticMesh/Box", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeSphere(vertexCollection, indexCollection, 1.0f, 16, false, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection);
		GetAssetManager()->Import(L"Engine/StaticMesh/Sphere", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeGeoSphere(vertexCollection, indexCollection, 1.0f, 5, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection);
		GetAssetManager()->Import(L"Engine/StaticMesh/GeoSphere", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeCylinder(vertexCollection, indexCollection, 1.0f, 1.0f, 16, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection);
		GetAssetManager()->Import(L"Engine/StaticMesh/Cylinder", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeCone(vertexCollection, indexCollection, 1.0f, 1.0f, 16, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection);
		GetAssetManager()->Import(L"Engine/StaticMesh/Cone", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeTorus(vertexCollection, indexCollection, 1.0f, 0.1f, 16, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection);
		GetAssetManager()->Import(L"Engine/StaticMesh/Torus", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeTetrahedron(vertexCollection, indexCollection, 1.0f, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection);
		GetAssetManager()->Import(L"Engine/StaticMesh/Tetrahedron", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeOctahedron(vertexCollection, indexCollection, 1.0f, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection);
		GetAssetManager()->Import(L"Engine/StaticMesh/Octahedron", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeDodecahedron(vertexCollection, indexCollection, 1.0f, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection);
		GetAssetManager()->Import(L"Engine/StaticMesh/Dodecahedron", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeIcosahedron(vertexCollection, indexCollection, 1.0f, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection);
		GetAssetManager()->Import(L"Engine/StaticMesh/Icosahedron", mesh);
	}

	{
		vector<RHIVertex> vertexCollection;
		vector<uint32> indexCollection;
		DirectXGeometry::ComputeTeapot(vertexCollection, indexCollection, 1.0f, 16, false);

		TRefPtr<StaticMesh> mesh = StaticMesh::CreateStaticMesh(vertexCollection, indexCollection);
		GetAssetManager()->Import(L"Engine/StaticMesh/Teapot", mesh);
	}
}