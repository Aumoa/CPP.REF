// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:Console;

export import :ConsoleColor;
export import :MulticastDelegate;
export import :TextWriter;

export class CORE_API Console
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