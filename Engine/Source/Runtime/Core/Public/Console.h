// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "ConsoleColor.h"
#include "MulticastDelegate.h"
#include "IO/TextWriter.h"

namespace Ayla
{
	class CORE_API Console
	{
	public:
		static TextWriter& Out;
		static TextWriter& Error;

	public:
		static void Write(String Str);
		static void WriteLine(String Str);

		DECLARE_MULTICAST_DELEGATE(CancelKeyPressedDelegate);
		static CancelKeyPressedDelegate CancelKeyPressed;

	public:
		template<class... TArgs>
		static void Write(String Format, TArgs&&... InArgs)
		{
			Write(String::Format(Format, std::forward<TArgs>(InArgs)...));
		}

		template<class... TArgs>
		static void WriteLine(String Format, TArgs&&... InArgs)
		{
			WriteLine(String::Format(Format, std::forward<TArgs>(InArgs)...));
		}
	};
}