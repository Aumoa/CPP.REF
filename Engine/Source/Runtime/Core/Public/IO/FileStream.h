// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IO/Stream.h"
#include "IO/FileAccessMode.h"
#include "IO/FileSharedMode.h"
#include "IO/FileMode.h"

class CORE_API FileStream : public Stream
{
private:
	void* _handle = nullptr;

public:
	FileStream(String filename, EFileAccessMode accessMode, EFileSharedMode sharedMode, EFileMode fileMode);
	virtual ~FileStream() noexcept override;

	virtual void Flush() override;
	virtual void Close() override;
	virtual void Write(std::span<const uint8> bytes) override;
	virtual Task<> WriteAsync(std::span<const uint8> bytes, std::stop_token cancellationToken = {}) override;
	virtual size_t Read(std::span<uint8> bytes) override;
	virtual Task<size_t> ReadAsync(std::span<uint8> bytes, std::stop_token cancellationToken = {}) override;
};