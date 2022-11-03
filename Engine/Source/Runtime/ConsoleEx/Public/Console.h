// Copyright 2017 CLOVERGAMES Co., Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Console.gen.h"

SCLASS()
class CONSOLEEX_API Console : implements Object
{
	GENERATED_BODY()

public:
	template<class... TArgs>
	static void Write(const String& format, TArgs&&... args)
	{
		Write(String::Format(format, std::forward<TArgs>(args)...));
	}

	template<class... TArgs>
	static void WriteLine(const String& format, TArgs&&... args)
	{
		Write(format + TEXT("\n"), std::forward<TArgs>(args)...);
	}

	static void Write(const String& format);
};