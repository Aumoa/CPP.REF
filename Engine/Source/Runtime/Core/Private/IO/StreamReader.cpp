// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/StreamReader.h"
#include "IO/Stream.h"
#include "Environment.h"

StreamReader::StreamReader(PolPtr<Stream> stream)
	: _stream(std::move(stream))
{
}

StreamReader::~StreamReader() noexcept
{
}

String StreamReader::ReadLine()
{
	auto search = +[](uint8* block, size_t len, const String& find) -> size_t
	{
		size_t sub = find.length() - 1;
		size_t hit = 0;
		for (size_t i = 0; i < len - sub; ++i)
		{
			if (block[i] == find[hit])
			{
				++hit;
			}
			if (hit == find.length())
			{
				return i - sub;
			}
		}

		return -1;
	};

	while (_eof() == false)
	{
		size_t idx = search(_cached.data() + _seekpos, _cached.size() - _seekpos, Environment::NewLine);
		if (idx == -1)
		{
			if (_cache_buf_async().GetResult() == false)
			{
				break;
			}
			continue;
		}

		String str = _to_string(idx - _seekpos, true);
		_seekpos += Environment::NewLine.length();
		return str;
	}

	return _to_string(_cached.size() - _seekpos, true);
}