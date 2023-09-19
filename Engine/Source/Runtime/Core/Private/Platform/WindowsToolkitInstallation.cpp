// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Platform/WindowsToolkitInstallation.h"
#include "IO/Directory.h"
#include "System/Path.h"

std::map<Version, std::shared_ptr<WindowsToolkitInstallation>> WindowsToolkitInstallation::CachedWindowsToolkitInstallations;

WindowsToolkitInstallation::WindowsToolkitInstallation(String InBaseDirectory, const Version& InInstallVersion)
	: BaseDirectory(InBaseDirectory)
	, InstallVersion(InInstallVersion)
{
}

String WindowsToolkitInstallation::GetExecutablePath() const
{
	return Path::Combine(BaseDirectory, TEXT("x64"));
}

std::shared_ptr<WindowsToolkitInstallation> WindowsToolkitInstallation::GetLatestInstallation()
{
	if (CachedWindowsToolkitInstallations.empty())
	{
		std::vector<String> VersionDirs = Directory::GetDirectories(TEXT("C:\\Program Files (x86)\\Windows Kits\\10\\bin"));
		for (auto& DirectoryPath : VersionDirs)
		{
			String VersionStr = Path::GetFileName(DirectoryPath);
			if (Version Ver; Version::TryParse(VersionStr, Ver))
			{
				auto Toolkit = std::shared_ptr<WindowsToolkitInstallation>(new WindowsToolkitInstallation(DirectoryPath, Ver));
				CachedWindowsToolkitInstallations.emplace(Ver, std::move(Toolkit));
			}
		}
	}

	if (CachedWindowsToolkitInstallations.empty())
	{
		return nullptr;
	}

	auto It = --CachedWindowsToolkitInstallations.end();
	return It->second;
}