// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "EngineSubsystems/GameAssetSystem.h"
#include "Misc/Paths.h"
#include "LogGame.h"
#include "Assets/Texture2D.h"
#include "Assets/Font.h"

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
	SearchDirectory(L"Game/Content");
	SearchDirectory(L"Engine/Content");
	SE_LOG(LogAssets, Verbose, L"{} content(s) found.", _assets.size());
}

SObject* SGameAssetSystem::LoadObject(const std::filesystem::path& assetPath)
{
	using namespace std::filesystem;

	auto it = _assets.find(assetPath);
	if (it == _assets.end())
	{
		// Pre-searched asset path is not found. Add path to contents list if file exists.
		if (!exists(assetPath))
		{
			SE_LOG(LogAssets, Error, L"Could not found asset from path: {}.", assetPath.wstring());
			return nullptr;
		}

		it = _assets.emplace(assetPath, nullptr).first;
	}

	// Load object.
	if (it->second == nullptr)
	{
		if (!exists(assetPath))
		{
			SE_LOG(LogAssets, Error, L"Could not found asset file from path: {}.", assetPath.wstring());
			return nullptr;
		}

		path ext = assetPath.extension();
		SObject* loadedObject = [&]() -> SObject*
		{
			if (auto loaded = LoadTexture2D(assetPath); loaded)
			{
				return loaded;
			}
			else if (auto loaded = LoadFont(assetPath); loaded)
			{
				return loaded;
			}
			else
			{
				return nullptr;
			}
		}();

		it->second = loadedObject;
	}

	return it->second;
}

void SGameAssetSystem::SearchDirectory(const std::filesystem::path& searchDirectory)
{
	using namespace std;
	using namespace std::filesystem;

	if (!exists(searchDirectory))
	{
		SE_LOG(LogAssets, Fatal, L"Content directory is not exists. Please check your work directory on debug settings.");
	}

	SE_LOG(LogAssets, Verbose, L"Search asset directory on {}", searchDirectory.wstring());

	queue<path> searchRecursivePaths;
	searchRecursivePaths.emplace(searchDirectory);

	for (path directory; searchRecursivePaths.size();)
	{
		directory = move(searchRecursivePaths.front());

		for (auto item : directory_iterator(directory))
		{
			path mypath = item.path();
			if (item.is_directory())
			{
				searchRecursivePaths.emplace(mypath);
			}
			else
			{
				_assets.emplace(mypath, nullptr);
			}
		}

		searchRecursivePaths.pop();
	}
}

STexture2D* SGameAssetSystem::LoadTexture2D(const std::filesystem::path& assetPath)
{
	constexpr std::array AllowExtensions =
	{
		L".png",
		L".jpg", L"jpeg",
		L".bmp",
		L".gif",
	};

	if (assetPath.has_extension())
	{
		auto ext = assetPath.extension();
		const bool bAllowed = std::find(AllowExtensions.begin(), AllowExtensions.end(), ext.wstring()) != AllowExtensions.end();
		if (bAllowed)
		{
			auto* object = NewObject<STexture2D>(assetPath);
			object->StreamIn();
			return object;
		}
	}

	return nullptr;
}

SFont* SGameAssetSystem::LoadFont(const std::filesystem::path& assetPath)
{
	constexpr std::array AllowExtensions =
	{
		L".ttf",
		L".ttc",
		L".fon",
	};

	if (assetPath.has_extension())
	{
		auto ext = assetPath.extension();
		const bool bAllowed = std::find(AllowExtensions.begin(), AllowExtensions.end(), ext.wstring()) != AllowExtensions.end();
		if (bAllowed)
		{
			auto* object = NewObject<SFont>(assetPath);
			object->StreamIn();
			return object;
		}
	}

	return nullptr;
}