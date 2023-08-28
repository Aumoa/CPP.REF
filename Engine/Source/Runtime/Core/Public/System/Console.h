// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "System/ConsoleColor.h"
#include "System/MulticastDelegate.h"
#include "IO/TextWriter.h"

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