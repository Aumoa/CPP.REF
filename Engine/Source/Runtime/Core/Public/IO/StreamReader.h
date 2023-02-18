// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IO/TextReader.h"
#include "Misc/PolPtr.h"
#include <vector>

class Stream;

class CORE_API StreamReader : public TextReader
{
private:
	PolPtr<Stream> _stream;
	std::vector<uint8> _cached;
	size_t _seekpos = 0;

public:
	StreamReader(PolPtr<Stream> stream);
	virtual ~StreamReader() noexcept override;

	virtual String ReadLine() override;
	virtual Task<String> ReadLineAsync(std::stop_token cancellationToken) override;
	virtual bool IsEOF() override;

private:
	Task<bool> _cache_buf_async() { throw; }
	bool _eof() { throw; }
	String _to_string(size_t length, bool incr) { throw; }
};