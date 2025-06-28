// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_LINUX

#include "LinuxStandardStreamTextWriter.h"
#include "AssertionMacros.h"

namespace Ayla
{
	LinuxStandardStreamTextWriter::LinuxStandardStreamTextWriter(int32 sd)
        : m_SD(sd)
	{
	}

	void LinuxStandardStreamTextWriter::Write(String value)
	{
        auto ws = value.AsCodepage();
        write(m_SD, ws.c_str(), ws.length());
	}
}

#endif