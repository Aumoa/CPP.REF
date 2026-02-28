// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_OSX

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/PlatformCommon.h"
#include "IO/TextWriter.h"

namespace Ayla
{
	class OSXStandardStreamTextWriter : public TextWriter
	{
    private:
        int32 m_SD = 0;

	public:
		OSXStandardStreamTextWriter(int32 sd);

		virtual void Write(String value) override;

		int32 GetNativeHandle() const { return m_SD; }
	};
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif
