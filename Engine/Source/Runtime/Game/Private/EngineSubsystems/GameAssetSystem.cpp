// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameAssetSystem.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "Misc/Paths.h"
#include "LogGame.h"
#include "GameEngine.h"
#include "AssetsLoader.h"
#include <stack>
#include <queue>

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

void SGameAssetSystem::Init()
{
	Assimp = NewObject<SAssetsLoader>(GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetRHIDevice());
	Assimp->SetOuter(this);

	SearchDirectory(L"Game/Content");
	SearchDirectory(L"Engine/Content");
	
	if (AssetsToImport.size())
	{
		SE_LOG(LogAssets, Verbose, L"{} native assets found.", AssetsToImport.size());
		ConvertNativeAssets();
	}

	SE_LOG(LogAssets, Verbose, L"{} contents found.", Assets.size());
}

SObject* SGameAssetSystem::LoadObject(const std::filesystem::path& assetPath)
{
	check(false);
	return nullptr;
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
				if (Mypath.extension() == L"sasset")
				{
					Assets.emplace(Mypath, nullptr);
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
		ContentPath.replace_extension(L".sasset");

		if (!Assimp->ConvertAssets(NativePath, ContentPath))
		{
			SE_LOG(LogAssets, Warning, L"Not supported native asset detected.");
			continue;
		}

		Assets.emplace(ContentPath, nullptr);
	}

	AssetsToImport.clear();
}