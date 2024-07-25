// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:TextWriter;

export import :IntegralTypes;
export import :Char;
export import :String;
export import :Task;

export class CORE_API TextWriter
{
protected:
	TextWriter();

public:
	virtual ~TextWriter() noexcept;

	virtual void Write(String Val) = 0;

	template<class... TArgs>
	void Write(String Format, TArgs&&... InArgs)
	{
		Write(String::Format(Format, std::forward<TArgs>(InArgs)...));
	}

	virtual Task<> WriteAsync(String Val)
	{
		Write(Val);
		return Task<>::CompletedTask();
	}

	template<class... TArgs>
	Task<> WriteAsync(String Format, TArgs&&... InArgs)
	{
		return WriteAsync(String::Format(Format, std::forward<TArgs>(InArgs)...));
	}

	virtual void WriteLine(String Val)
	{
		Write(Val + TEXT("\n"));
	}

	template<class... TArgs>
	void WriteLine(String Format, TArgs&&... InArgs)
	{
		WriteLine(String::Format(Format, std::forward<TArgs>(InArgs)...));
	}

	virtual Task<> WriteLineAsync(String Val)
	{
		WriteLine(Val);
		return Task<>::CompletedTask();
	}

	template<class... TArgs>
	Task<> WriteLineAsync(String Format, TArgs&&... InArgs)
	{
		return WriteLineAsync(String::Format(Format, std::forward<TArgs>(InArgs)...));
	}

protected:
	char_t CoreNewLine;
};