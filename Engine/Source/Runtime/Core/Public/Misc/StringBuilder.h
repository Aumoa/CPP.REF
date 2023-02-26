// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreTypes/String.h"
#include <vector>

class CORE_API StringBuilder
{
	std::vector<String> _sources;
	size_t _length;

public:
	StringBuilder() noexcept;

	void Append(const String& str);
	void AppendLine(const String& str);
	void Insert(size_t idx, const String& str);
	void Clear() noexcept;

	inline void AppendLine()
	{
		AppendLine(TEXT(""));
	}

	inline void AppendLineFormat()
	{
		AppendLine(TEXT(""));
	}

	template<class... TArgs>
	inline void AppendFormat(const String& format, TArgs&&... args)
	{
		Append(String::Format(format, std::forward<TArgs>(args)...));
	}

	template<class... TArgs>
	inline void AppendLineFormat(const String& format, TArgs&&... args)
	{
		AppendLine(String::Format(format, std::forward<TArgs>(args)...));
	}

	template<class... TArgs>
	inline void InsertFormat(const String& format, TArgs&&... args)
	{
		Insert(String::Format(format, std::forward<TArgs>(args)...));
	}

	[[nodiscard]] inline String ToString()
	{
		this->_Generate_string();
		if (_sources.empty())
		{
			return TEXT("");
		}
		else
		{
			return _sources.front();
		}
	}

	[[nodiscard]] size_t GetLength() const noexcept
	{
		return _length;
	}

private:
	void _Generate_string();
};