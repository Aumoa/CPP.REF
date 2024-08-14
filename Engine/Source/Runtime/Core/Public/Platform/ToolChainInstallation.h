// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/String.h"
#include "System/Version.h"

class CORE_API ToolChainInstallation
{
protected:
	ToolChainInstallation() = default;

public:
	virtual ~ToolChainInstallation() noexcept = default;

	virtual String GetBaseDirectory() const = 0;
	virtual Version GetInstallVersion() const = 0;
	virtual String ToString() const;
	virtual String GetExecutablePath() const = 0;
};