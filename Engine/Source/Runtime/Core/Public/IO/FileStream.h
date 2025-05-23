// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IO/Stream.h"
#include "IO/FileMode.h"
#include "IO/FileAccessMode.h"
#include "IO/FileSharedMode.h"
#include "Platform/PlatformIO.h"

namespace Ayla
{
	class CORE_API FileStream : public Stream
	{
	private:
		void* FileHandle = nullptr;
		uint8 IOCPReadBuf[PlatformIO::OVERLAPPED_SIZE] = {};
		uint8 IOCPWriteBuf[PlatformIO::OVERLAPPED_SIZE] = {};

	public:
		FileStream(String InPath, FileMode InFileMode, FileAccessMode InAccessMode, FileSharedMode InSharedMode = FileSharedMode::None);
		virtual ~FileStream() noexcept override;

		virtual void Flush() override;
		virtual void Close() override;

		virtual void Write(std::span<const uint8> InBytes) override;
		virtual Task<> WriteAsync(std::span<const uint8> InBytes, CancellationToken InCancellationToken = {}) override;
		virtual void Seek(int64 InSeekpos, SeekOrigin InOrigin) override;

		virtual size_t Read(std::span<uint8> OutBytes) override;
		virtual Task<size_t> ReadAsync(std::span<uint8> OutBytes, CancellationToken InCancellationToken = {}) override;
		virtual size_t GetLength() const override;
	};
}