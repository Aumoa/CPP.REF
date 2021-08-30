// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "EngineSubsystems/GameAssetSystem.h"
#include "Misc/Paths.h"
#include "LogGame.h"

template<class T>
inline static auto pop_get(std::stack<T>& container)
{
	T value = container.top();
	container.pop();
	return value;
}

GameAssetSystem::GameAssetSystem() : Super()
{
}

GameAssetSystem::~GameAssetSystem()
{
}

void GameAssetSystem::Init()
{
	using namespace std;
	using namespace std::filesystem;

	path searchDirectory = Paths::GetContentPath();
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

	SE_LOG(LogAssets, Verbose, L"{} content(s) found.", _assets.size());
}

Object* GameAssetSystem::LoadObject(const std::filesystem::path& assetPath)
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
		std::ifstream binaryReader(it->first, std::ios::binary);
		if (!binaryReader.is_open())
		{
			SE_LOG(LogAssets, Error, L"Could not open asset file from path: {}.", assetPath.wstring());
			return nullptr;
		}

		// TODO: Read file.
		binaryReader.close();
	}

	return it->second;
}