// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Engine.h"

#include "DirectXGeometry.h"
#include "Assets/AssetManager.h"
#include "SceneRendering/StaticMesh.h"
#include "Materials/Material.h"
#include "RHI/RHICommon.h"
#include "RHI/IRHIMaterialBundle.h"
#include "RHI/IRHIResourceBundle.h"
#include "Logging/LogMacros.h"

#include "RHI/IRHIDeviceBundle.h"
#include "RHI/IRHIResource.h"
#include "PlatformMisc/PlatformImageLoader.h"
#include "PlatformMisc/PlatformBitmapFrame.h"

using namespace std;

//inline void LoadEngineDefaultMaterials(Engine* engine)
//{
//	auto bundle = engine->MaterialBundle;
//	auto mgr = engine->GetAssetManager();
//
//	{
//		auto imageLoader = engine->NewObject<PlatformImageLoader>(L"img.jpg");
//		auto imageFrame = imageLoader->GetFrame(0);
//		auto imageConv = imageFrame->FormatConvert(ERHITextureFormat::B8G8R8A8_UNORM);
//		auto diffuseMap = engine->ResourceBundle->CreateTexture2D(ERHITextureFormat::B8G8R8A8_UNORM, imageConv.Get());
//
//		TRefPtr<MaterialInterface> material = bundle->CreateMaterial();
//		material->Ambient = 0.2f;
//		material->Diffuse = 0.5f;
//		material->Specular = 0.8f;
//		material->SpecExp = 32.0f;
//		material->DiffuseMap = diffuseMap.Get();
//		mgr->Import(L"Engine/Materials/Default", move(material).As<Material>());
//	}
//}
//
//inline void LoadEngineDefaultStaticMeshes(Engine* engine)
//{
//	auto mgr = engine->GetAssetManager();
//	auto mat = mgr->LoadMaterial(L"Engine/Materials/Default");
//	auto resourceBundle = engine->ResourceBundle;
//
//	auto RegisterSingleSubsetGeometry = [&](wstring_view importPath, const vector<RHIVertex>& vc, const vector<uint32>& ic)
//	{
//		RHIStaticMeshSubsetInfo subset[1] = { };
//		subset[0].VertexStart = 0;
//		subset[0].VertexCount = (uint32)vc.size();
//		subset[0].IndexStart = 0;
//		subset[0].IndexCount = (uint32)ic.size();
//
//		Material* mats[1] = { mat };
//
//		RHIStaticMeshGeometryData data;
//		data.VertexBuffer = vc;
//		data.IndexBuffer = ic;
//		data.Subsets = subset;
//		data.Materials = mats;
//
//		TRefPtr<StaticMesh> mesh = resourceBundle->CreateStaticMesh(data);
//		mgr->Import(importPath.data(), mesh);
//	};
//
//	{
//		vector<RHIVertex> vertexCollection;
//		vector<uint32> indexCollection;
//		DirectXGeometry::ComputeBox(vertexCollection, indexCollection, 1.0f, false, false);
//		RegisterSingleSubsetGeometry(L"Engine/StaticMesh/Box", vertexCollection, indexCollection);
//	}
//
//	{
//		vector<RHIVertex> vertexCollection;
//		vector<uint32> indexCollection;
//		DirectXGeometry::ComputeSphere(vertexCollection, indexCollection, 1.0f, 16, false, false);
//		RegisterSingleSubsetGeometry(L"Engine/StaticMesh/Sphere", vertexCollection, indexCollection);
//	}
//
//	{
//		vector<RHIVertex> vertexCollection;
//		vector<uint32> indexCollection;
//		DirectXGeometry::ComputeGeoSphere(vertexCollection, indexCollection, 1.0f, 4, false);
//		RegisterSingleSubsetGeometry(L"Engine/StaticMesh/GeoSphere", vertexCollection, indexCollection);
//	}
//
//	{
//		vector<RHIVertex> vertexCollection;
//		vector<uint32> indexCollection;
//		DirectXGeometry::ComputeCylinder(vertexCollection, indexCollection, 1.0f, 1.0f, 16, false);
//		RegisterSingleSubsetGeometry(L"Engine/StaticMesh/Cylinder", vertexCollection, indexCollection);
//	}
//
//	{
//		vector<RHIVertex> vertexCollection;
//		vector<uint32> indexCollection;
//		DirectXGeometry::ComputeCone(vertexCollection, indexCollection, 1.0f, 1.0f, 16, false);
//		RegisterSingleSubsetGeometry(L"Engine/StaticMesh/Cone", vertexCollection, indexCollection);
//	}
//
//	{
//		vector<RHIVertex> vertexCollection;
//		vector<uint32> indexCollection;
//		DirectXGeometry::ComputeTetrahedron(vertexCollection, indexCollection, 1.0f, false);
//		RegisterSingleSubsetGeometry(L"Engine/StaticMesh/Tetrahedron", vertexCollection, indexCollection);
//	}
//
//	{
//		vector<RHIVertex> vertexCollection;
//		vector<uint32> indexCollection;
//		DirectXGeometry::ComputeOctahedron(vertexCollection, indexCollection, 1.0f, false);
//		RegisterSingleSubsetGeometry(L"Engine/StaticMesh/Octahedron", vertexCollection, indexCollection);
//	}
//
//	{
//		vector<RHIVertex> vertexCollection;
//		vector<uint32> indexCollection;
//		DirectXGeometry::ComputeDodecahedron(vertexCollection, indexCollection, 1.0f, false);
//		RegisterSingleSubsetGeometry(L"Engine/StaticMesh/Dodecahedron", vertexCollection, indexCollection);
//	}
//
//	{
//		vector<RHIVertex> vertexCollection;
//		vector<uint32> indexCollection;
//		DirectXGeometry::ComputeIcosahedron(vertexCollection, indexCollection, 1.0f, false);
//		RegisterSingleSubsetGeometry(L"Engine/StaticMesh/Icosahedron", vertexCollection, indexCollection);
//	}
//
//	{
//		vector<RHIVertex> vertexCollection;
//		vector<uint32> indexCollection;
//		DirectXGeometry::ComputeTeapot(vertexCollection, indexCollection, 1.0f, 16, false);
//		RegisterSingleSubsetGeometry(L"Engine/StaticMesh/Teapot", vertexCollection, indexCollection);
//	}
//}
//
//void Engine::LoadEngineDefaultAssets()
//{
//	SE_LOG(LogEngine, Display, L"Load engine default assets.");
//	LoadEngineDefaultMaterials(this);
//	LoadEngineDefaultStaticMeshes(this);
//}