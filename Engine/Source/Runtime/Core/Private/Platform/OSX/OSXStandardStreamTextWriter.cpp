// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_OSX

#include "OSXStandardStreamTextWriter.h"
#include "AssertionMacros.h"

namespace Ayla
{
	OSXStandardStreamTextWriter::OSXStandardStreamTextWriter(int32 sd)
        : m_SD(sd)
	{
	}

	void OSXStandardStreamTextWriter::Write(String value)
	{
        auto ws = value.AsCodepage();
        write(m_SD, ws.c_str(), ws.length());
	}
}

#endif
