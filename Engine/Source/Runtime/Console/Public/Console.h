// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <mutex>

class CONSOLE_API Console : public AbstractClass
{
private:
	static std::mutex Locker;

public:
	static void Write(std::wstring_view Message);
	static void WriteLine(std::wstring_view Message);
	static std::wstring ReadLine();
	static void Clear();

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
		if (InArg)
		{
			return InArg->ToString();
		}
		else
		{
			return L"";
		}
	}

	template<template<class> class TSmartPtr, std::derived_from<SObject> T>
	static auto TransformArg(const TSmartPtr<T>& InArg)
	{
		if (auto Ptr = InArg.Get())
		{
			return Ptr->ToString();
		}
		else
		{
			return L"";
		}
	}

	template<class T>
	static auto TransformArg(T&& InArg)
	{
		return std::forward<T>(InArg);
	}
};