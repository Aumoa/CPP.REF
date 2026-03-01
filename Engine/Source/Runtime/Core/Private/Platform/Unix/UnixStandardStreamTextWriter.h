// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/PlatformCommon.h"
#include "IO/TextWriter.h"
#include <unistd.h>

namespace Ayla
{
	class UnixStandardStreamTextWriter : public TextWriter
	{
	private:
		int32 m_SD = 0;

	public:
		UnixStandardStreamTextWriter(int32 sd)
			: m_SD(sd)
		{
		}

		virtual void Write(String value) override
		{
			auto ws = value.AsCodepage();
			write(m_SD, ws.c_str(), ws.length());
		}

		int32 GetNativeHandle() const { return m_SD; }
	};
}

#undef __ALLOW_PLATFORM_COMMON_H__
