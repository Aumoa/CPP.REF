// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.IO:StreamReader;

export import Core.Std;
export import Core.System;
export import :TextReader;
export import :Stream;

export class CORE_API StreamReader : public TextReader
{
	static constexpr size_t InitialBufferSize = 1024;

	std::shared_ptr<Stream> CurrentStream;
	std::vector<uint8> Buffer;
	size_t BufferPos = 0;
	bool bEOF = false;

public:
	StreamReader(std::shared_ptr<Stream> InStream);

	virtual String ReadLine() override;
	virtual Task<String> ReadLineAsync(std::stop_token InCancellationToken = {}) override;
	virtual Task<String> ReadToEndAsync(std::stop_token InCancellationToken = {}) override;
	virtual bool IsEOF() const override;

private:
	Task<> TryShrinkAndFillAsync(std::stop_token InCancellationToken);
	Task<bool> ExpandBufferAsync(std::stop_token InCancellationToken);
};