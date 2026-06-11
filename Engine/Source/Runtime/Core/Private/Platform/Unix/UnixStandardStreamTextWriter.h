// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/PlatformCommon.h"
#include "IO/TextWriter.h"
#include <cerrno>
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
			const char* cursor = ws.c_str();
			size_t remaining = ws.length();
			while (remaining > 0)
			{
				ssize_t written = write(m_SD, cursor, remaining);
				if (written < 0)
				{
					if (errno == EINTR)
					{
						continue;
					}

					break;
				}

				if (written == 0)
				{
					break;
				}

				cursor += written;
				remaining -= (size_t)written;
			}
		}

		int32 GetNativeHandle() const { return m_SD; }
	};
}

#undef __ALLOW_PLATFORM_COMMON_H__
