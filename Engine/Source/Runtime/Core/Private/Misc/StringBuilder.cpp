// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/StringBuilder.h"
#include "Exceptions/ArgumentException.h"

using namespace ::libty;

StringBuilder::StringBuilder()
	: _length(0)
{
}

void StringBuilder::Append(String str)
{
	_sources.emplace_back(str);
	_length += (size_t)str;
}

void StringBuilder::AppendLine(String str)
{
	_sources.emplace_back(str);
	_sources.emplace_back(TEXT("\n"));
	_length += (size_t)str + 1;
}

void StringBuilder::Insert(size_t idx, String str)
{
	if (idx > _length)
	{
		throw ArgumentException(String::Format(TEXT("Index overflow detected. idx[{}] > length[{}]"), idx, _length));
	}

	String generated = ToString();
	_sources.clear();
	
	_sources.emplace_back(generated.Substring(0, idx));
	_sources.emplace_back(str);
	_sources.emplace_back(generated.Substring(idx));
}

void StringBuilder::Clear() noexcept
{
	_sources.clear();
	_length = 0;
}

void StringBuilder::_Generate_string()
{
	if (!_sources.empty())
	{
		String str = String::Concat(_sources);
		_sources.clear();
		_sources.emplace_back(str);
	}
}