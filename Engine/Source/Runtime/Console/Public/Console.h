// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class CONSOLE_API Console : public AbstractClass
{
public:
	static void Write(std::wstring_view Message);
	static void WriteLine(std::wstring_view Message);

	template<class... TArgs>
	static void Write(std::wstring_view Message, TArgs&&... InFormatArgs)
	{
		Write(std::format(Message, TransformArg<TArgs>(std::forward<TArgs>(InFormatArgs))...));
	}

	template<class... TArgs>
	static void WriteLine(std::wstring_view Message, TArgs&&... InFormatArgs)
	{
		WriteLine(std::format(Message, TransformArg<TArgs>(std::forward<TArgs>(InFormatArgs))...));
	}

private:
	template<std::derived_from<SObject> T>
	static auto TransformArg(T&& InArg)
	{
		return InArg->ToString();
	}

	template<class T>
	static auto TransformArg(T&& InArg)
	{
		return std::forward<T>(InArg);
	}
};