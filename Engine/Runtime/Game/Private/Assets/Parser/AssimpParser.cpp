// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "AssimpParser.h"

#include "Engine.h"
#include "Path.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Logging/LogMacros.h"
#include "SceneRendering/StaticMesh.h"
#include "SceneRendering/Vertex.h"
#include "Materials/Material.h"
#include "COM/COMImageLoader.h"
#include "COM/COMBitmapFrame.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"

using namespace std;

AssimpParser::AssimpParser(Engine* engine) : Super()
	, engine(engine)
	, myScene(nullptr)
{
	myImporter = make_unique<Assimp::Importer>();
}

AssimpParser::~AssimpParser()
{

}

bool AssimpParser::TryParse(TRefPtr<String> filepath)
{
	static constexpr int32 ReadOptions =
		aiProcess_JoinIdenticalVertices |
		aiProcess_ImproveCacheLocality |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_GenUVCoords |
		aiProcess_CalcTangentSpace |
		aiProcess_LimitBoneWeights |
		aiProcess_GenNormals |
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals;

	string filepath_mb = filepath->AsMultiByte();
	myScene = myImporter->ReadFile(filepath_mb.c_str(), ReadOptions);
	if (myScene == nullptr)
	{
		SE_LOG(LogAssets, Error, L"Cannot parsing scene from {0} file with assimp library.", filepath);
		return false;
	}

	// Save directory name.
	directoryName = Path::GetDirectoryName(filepath);

	// Check that mesh has any bone.
	bool bSkeletalMesh = false;
	for (uint32 i = 0; i < myScene->mNumMeshes; ++i)
	{
		if (myScene->mMeshes[i]->HasBones())
		{
			bSkeletalMesh = true;
			break;
		}
	}

	if (!ProcessMaterials())
	{
		SE_LOG(LogAssets, Error, L"Cannot process materials.");
		return false;
	}

	if (bSkeletalMesh)
	{
		SE_LOG(LogAssets, Error, L"SkeletalMesh is not supported yet with assimp library.");
		//return false;
	}
	//else
	{
		if (!ProcessStaticMeshSubsets())
		{
			SE_LOG(LogAssets, Error, L"Cannot process static mesh subsets.");
			return false;
		}
	}

	return true;
}

bool AssimpParser::IsStaticMesh() const
{
	return staticMesh.IsValid;
}

TRefPtr<StaticMesh> AssimpParser::GetStaticMesh() const
{
	return staticMesh;
}

bool AssimpParser::ProcessMaterials()
{
	DirectXDeviceBundle* deviceBundle = engine->GetDeviceBundle();
	COMDeviceBundle* comBundle = engine->GetCOMDevice();

	materials.reserve(myScene->mNumMaterials);
	for (uint32 i = 0; i < myScene->mNumMaterials; ++i)
	{
		aiMaterial* aiMat = myScene->mMaterials[i];

		TRefPtr<Material>& mat = materials.emplace_back();
		mat = NewObject<Material>(engine->GetDeviceBundle());

		auto aiMat_GetColor3D = [aiMat](auto key, auto _1, auto _2, const Vector3& _default = Vector3::Zero) -> Vector3
		{
			aiColor3D value;
			if (aiMat->Get(key, _1, _2, value) != AI_SUCCESS)
			{
				return _default;
			}
			else
			{
				return reinterpret_cast<Vector3&>(value);
			}
		};

		auto aiMat_GetFloat = [aiMat](auto key, auto _1, auto _2, float _default = 0) -> float
		{
			ai_real value;
			if (aiMat->Get(key, _1, _2, value) != AI_SUCCESS)
			{
				return _default;
			}
			else
			{
				return value;
			}
		};

		auto aiMat_LoadTexture = [&](auto aiTextureType, int32 index) -> TComPtr<ID3D12Resource>
		{
			aiString diffuseId;
			if (aiMat->Get(AI_MATKEY_TEXTURE(aiTextureType, index), diffuseId) != AI_SUCCESS)
			{
				return nullptr;
			}

			TRefPtr<String> fullpath = String::Format(L"{0}/{1}", directoryName, diffuseId.C_Str());
			if (optional<size_t> index = fullpath->IndexOf(L'*'); index.has_value())
			{
				SE_LOG(LogAssets, Verbose, L"Filepath include asterlisk detected. Cut path. Original: {0}", fullpath);
				fullpath = fullpath->Substring(0, index.value());
			}
			
			auto imageLoader = NewObject<COMImageLoader>(comBundle, fullpath);
			auto imageFrame = imageLoader->GetFrame(0); 
			auto converter = imageFrame->ConvertFormat(DXGI_FORMAT_R8G8B8A8_UNORM);

			return deviceBundle->CreateTexture2D(converter.Get(), engine->GetPrimaryCommandQueue(), DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		};

		// Assign color keys.
		mat->Ambient = aiMat_GetColor3D(AI_MATKEY_COLOR_AMBIENT);
		mat->Diffuse = aiMat_GetColor3D(AI_MATKEY_COLOR_DIFFUSE);
		mat->Specular = aiMat_GetColor3D(AI_MATKEY_COLOR_SPECULAR);
		mat->SpecExp = aiMat_GetFloat(AI_MATKEY_SHININESS);

		mat->DiffuseMap = aiMat_LoadTexture(aiTextureType_DIFFUSE, 0).Get();
		mat->NormalMap = aiMat_LoadTexture(aiTextureType_NORMALS, 0).Get();
	}

	return true;
}

bool AssimpParser::ProcessStaticMeshSubsets()
{
	vector<Vertex> vertexBuffer;
	vector<uint32> indexBuffer;
	vector<StaticMeshSubsetInfo> subsets;

	// Query size for all containers preallocate.
	size_t vertexCount = 0;
	size_t indexCount = 0;
	size_t subsetCount = 0;
	for (uint32 i = 0; i < myScene->mNumMeshes; ++i)
	{
		const aiMesh* myMesh = myScene->mMeshes[i];
		vertexCount += myMesh->mNumVertices;
		indexCount += myMesh->mNumFaces * 3;
		subsetCount += 1;
	}

	// Preallocate containers.
	vertexBuffer.reserve(vertexCount);
	indexBuffer.reserve(indexCount);
	subsets.reserve(subsetCount);

	// Store all elements for this game engine.
	auto StoreSingleMesh = [&](const aiMesh* inMesh)
	{
		// Store subset.
		StaticMeshSubsetInfo& subset = subsets.emplace_back();
		subset.VertexStart = (uint32)vertexBuffer.size();
		subset.IndexStart = (uint32)indexBuffer.size();
		subset.VertexCount = (uint32)inMesh->mNumVertices;
		subset.IndexCount = (uint32)inMesh->mNumFaces * 3;
		subset.Material = materials[inMesh->mMaterialIndex].Get();

		// Store vertices.
		for (uint32 i = 0; i < inMesh->mNumVertices; ++i)
		{
			Vertex& myVertex = vertexBuffer.emplace_back();
			memcpy(&myVertex.Pos, &inMesh->mVertices[i], sizeof(Vector3));
			memcpy(&myVertex.Normal, &inMesh->mNormals[i], sizeof(Vector3));
			memcpy(&myVertex.TexCoord, &inMesh->mTextureCoords[0][i], sizeof(Vector2));
		}

		// Store indices.
		for (uint32 i = 0; i < inMesh->mNumFaces; ++i)
		{
			uint32& _1 = indexBuffer.emplace_back();
			indexBuffer.emplace_back();
			indexBuffer.emplace_back();

			memcpy(&_1, inMesh->mFaces[i].mIndices, sizeof(uint32) * 3);
		}
	};

	for (uint32 i = 0; i < myScene->mNumMeshes; ++i)
	{
		StoreSingleMesh(myScene->mMeshes[i]);
	}

	// Create static mesh.
	StaticMeshGeometryData geometryData;
	geometryData.VertexBuffer = move(vertexBuffer);
	geometryData.IndexBuffer = move(indexBuffer);
	geometryData.Subsets = move(subsets);

	staticMesh = NewObject<StaticMesh>(engine, geometryData);
	return true;
}