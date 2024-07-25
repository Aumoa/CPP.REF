// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:TextReader;

export import :String;
export import :Task;
export import :CancellationToken;

export class TextReader
{
protected:
	TextReader() = default;

public:
	virtual ~TextReader() noexcept = default;

	virtual String ReadLine() = 0;
	virtual Task<String> ReadLineAsync(CancellationToken InCancellationToken = {}) = 0;
	virtual Task<String> ReadToEndAsync(CancellationToken InCancellationToken = {}) = 0;
	virtual bool IsEOF() const = 0;
};