// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreTypes/CharType.h"
#include "CoreTypes/IntegralTypes.h"
#include "CoreTypes/RealTypes.h"
#include "CoreTypes/String.h"

class CORE_API TextWriter
{
protected:
	TextWriter();

public:
	virtual ~TextWriter() noexcept;

	virtual void Write(char_t Val) = 0;
	virtual void Write(String Val);
	virtual void Write(bool bVal);
	virtual void Write(int32 Val);
	virtual void Write(uint32 Val);
	virtual void Write(int64 Val);
	virtual void Write(uint64 Val);
	virtual void Write(float Val);
	virtual void Write(double Val);

	template<class... TArgs>
	void Write(String Format, TArgs&&... InArgs)
	{
		Write(String::Format(Format, std::forward<TArgs>(InArgs)...));
	}

	virtual void WriteLine();
	virtual void WriteLine(char_t Val);
	virtual void WriteLine(String Val);
	virtual void WriteLine(bool bVal);
	virtual void WriteLine(int32 Val);
	virtual void WriteLine(uint32 Val);
	virtual void WriteLine(int64 Val);
	virtual void WriteLine(uint64 Val);
	virtual void WriteLine(float Val);
	virtual void WriteLine(double Val);

	template<class... TArgs>
	void WriteLine(String Format, TArgs&&... InArgs)
	{
		WriteLine(String::Format(Format, std::forward<TArgs>(InArgs)...));
	}

protected:
	char_t CoreNewLine;
};