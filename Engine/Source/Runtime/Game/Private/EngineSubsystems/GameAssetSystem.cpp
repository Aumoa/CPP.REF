// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameAssetSystem.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "LogGame.h"
#include "GameEngine.h"
#include "AssetsLoader.h"
#include <stack>
#include <queue>

GENERATE_BODY(SGameAssetSystem);

template<class T>
inline static auto pop_get(std::stack<T>& container)
{
	T value = container.top();
	container.pop();
	return value;
}

SGameAssetSystem::SGameAssetSystem() : Super()
{
}

SGameAssetSystem::~SGameAssetSystem()
{
}

void SGameAssetSystem::PostInit()
{
	Assimp = NewObject<SAssetsLoader>(GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetRHIDevice());

	SearchDirectory(L"Content");
	
	if (AssetsToImport.size())
	{
		SE_LOG(LogAssets, Verbose, L"{} native assets found.", AssetsToImport.size());
		ConvertNativeAssets();
	}

	SE_LOG(LogAssets, Verbose, L"{} contents found.", Assets.size());
}

SObject* SGameAssetSystem::LoadObject(const std::filesystem::path& AssetPath)
{
	auto It = Assets.find(AssetPath);
	if (It == Assets.end())
	{
		SE_LOG(LogAssets, Error, L"Could not found asset({}) from assets tree.", AssetPath.wstring());
		return nullptr;
	}

	if (!It->second.IsValid())
	{
		SObject* ImportObject = nullptr;

		if (!AssetPath.has_extension())
		{
			ImportObject = Assimp->ImportFromFile(std::filesystem::path(AssetPath).replace_extension(L".sasset"));
		}
		else
		{
			ImportObject = Assimp->ImportFromFile(AssetPath);
		}

		if (ImportObject == nullptr)
		{
			SE_LOG(LogAssets, Error, L"Could not import asset: {}", AssetPath.wstring());
			return nullptr;
		}

		// Outer will exchange to another object.
		It->second = ImportObject;
	}

	return It->second.Get();
}

void SGameAssetSystem::SearchDirectory(const std::filesystem::path& SearchDirectory)
{
	using namespace std;
	using namespace std::filesystem;

	if (!exists(SearchDirectory))
	{
		SE_LOG(LogAssets, Fatal, L"Content directory({}) is not exists. Please check your work directory on debug settings.", SearchDirectory.wstring());
	}

	SE_LOG(LogAssets, Verbose, L"Search asset directory on {}", SearchDirectory.wstring());

	queue<path> SearchRecursivePaths;
	SearchRecursivePaths.emplace(SearchDirectory);

	for (path Directory; SearchRecursivePaths.size();)
	{
		Directory = move(SearchRecursivePaths.front());

		for (auto Item : directory_iterator(Directory))
		{
			path Mypath = Item.path();
			if (Item.is_directory())
			{
				SearchRecursivePaths.emplace(Mypath);
			}
			else
			{
				if (Mypath.extension() == L".sasset")
				{
					Assets.emplace(Mypath.replace_extension(), nullptr);
				}
				else
				{
					AssetsToImport.emplace(Mypath);
				}
			}
		}

		SearchRecursivePaths.pop();
	}
}

void SGameAssetSystem::ConvertNativeAssets()
{
	for (auto& NativePath : AssetsToImport)
	{
		std::filesystem::path ContentPath = NativePath;
		if (Assets.contains(ContentPath.replace_extension()))
		{
			continue;
		}

		if (!Assimp->ConvertAssets(NativePath, ContentPath.replace_extension(L".sasset")))
		{
			continue;
		}

		Assets.emplace(ContentPath, nullptr);
	}

	AssetsToImport.clear();
}