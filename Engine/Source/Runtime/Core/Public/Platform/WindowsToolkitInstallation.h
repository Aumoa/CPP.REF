// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/ToolChainInstallation.h"
#include "System/Version.h"
#include <map>

class CORE_API WindowsToolkitInstallation : public ToolChainInstallation
{
private:
	static std::map<Version, std::shared_ptr<WindowsToolkitInstallation>> CachedWindowsToolkitInstallations;

	String BaseDirectory;
	Version InstallVersion;

private:
	WindowsToolkitInstallation(String InBaseDirectory, const Version& InInstallVersion);

public:
	virtual ~WindowsToolkitInstallation() noexcept = default;

	virtual String GetBaseDirectory() const override { return BaseDirectory; }
	virtual Version GetInstallVersion() const override { return InstallVersion; }
	virtual String GetExecutablePath() const override;

public:
	static std::shared_ptr<WindowsToolkitInstallation> GetLatestInstallation();
};