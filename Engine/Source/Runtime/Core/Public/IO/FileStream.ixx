// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.IO:FileStream;

import Core.Std;
import :Stream;
import :FileMode;
import :FileAccessMode;
import :FileSharedMode;
import :SeekOrigin;

export class CORE_API FileStream : public Stream
{
private:
	void* FileHandle = nullptr;
	void* WriteOverlapPointer = nullptr;
	void* ReadOverlapPointer = nullptr;

public:
	FileStream(String InPath, EFileMode InFileMode, EFileAccessMode InAccessMode, EFileSharedMode InSharedMode = EFileSharedMode::None);
	virtual ~FileStream() noexcept override;

	virtual void Flush() override;
	virtual void Close() override;

	virtual void Write(std::span<const uint8> InBytes) override;
	virtual Task<> WriteAsync(std::span<const uint8> InBytes, std::stop_token InCancellationToken = {}) override;
	virtual void Seek(int64 InSeekpos, ESeekOrigin InOrigin) override;

	virtual size_t Read(std::span<uint8> OutBytes) override;
	virtual Task<size_t> ReadAsync(std::span<uint8> OutBytes, std::stop_token InCancellationToken = {}) override;
	virtual size_t GetLength() const override;
};