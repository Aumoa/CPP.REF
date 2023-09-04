// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_LINUX

#include "IO/TextWriter.h"

class LinuxStandardStreamTextWriter : public TextWriter
{
private:
	int32 IO;

public:
	LinuxStandardStreamTextWriter(int32 IO);

	virtual void Write(String Val) override;
};

#endif