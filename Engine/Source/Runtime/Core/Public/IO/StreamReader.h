// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "IO/TextReader.h"
#include "IO/Stream.h"

class CORE_API StreamReader : public TextReader
{
	static constexpr size_t InitialBufferSize = 1024;

	std::shared_ptr<Stream> CurrentStream_Capture;
	Stream* CurrentStream = nullptr;
	std::vector<uint8> Buffer;
	size_t BufferPos = 0;
	bool bEOF = false;

public:
	StreamReader(std::shared_ptr<Stream> InStream);
	StreamReader(Stream* InStreamRaw);

	virtual String ReadLine() override;
	virtual Task<String> ReadLineAsync(CancellationToken InCancellationToken = {}) override;
	virtual Task<String> ReadToEndAsync(CancellationToken InCancellationToken = {}) override;
	virtual bool IsEOF() const override;

private:
	Task<> TryShrinkAndFillAsync(CancellationToken InCancellationToken);
	Task<bool> ExpandBufferAsync(CancellationToken InCancellationToken);
};