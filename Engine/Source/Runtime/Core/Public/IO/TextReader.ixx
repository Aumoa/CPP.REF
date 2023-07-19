// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.IO:TextReader;

export import Core.Std;
export import Core.System;
export import Core.Threading;

export class TextReader
{
protected:
	TextReader() = default;

public:
	virtual ~TextReader() noexcept = default;

	virtual String ReadLine() = 0;
	virtual Task<String> ReadLineAsync(std::stop_token InCancellationToken = {}) = 0;
	virtual Task<String> ReadToEndAsync(std::stop_token InCancellationToken = {}) = 0;
	virtual bool IsEOF() const = 0;
};