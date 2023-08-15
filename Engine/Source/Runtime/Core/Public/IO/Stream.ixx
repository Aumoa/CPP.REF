// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.IO:Stream;

export import Core.Std;
export import Core.System;
export import Core.Threading;
export import :SeekOrigin;
export import :FileAccessMode;
export import :FileSharedMode;
export import :FileMode;

export class CORE_API Stream
{
protected:
	Stream() = default;

public:
	virtual ~Stream() noexcept = default;

	virtual void Flush() = 0;
	virtual void Close() = 0;

	virtual void Write(std::span<const uint8> InBytes) = 0;
	virtual Task<> WriteAsync(std::span<const uint8> InBytes, std::stop_token InCancellationToken = {}) = 0;
	virtual void Seek(int64 InSeekpos, ESeekOrigin InOrigin = ESeekOrigin::Begin) = 0;

	virtual size_t Read(std::span<uint8> OutBytes) = 0;
	virtual Task<size_t> ReadAsync(std::span<uint8> OutBytes, std::stop_token InCancellationToken = {}) = 0;
	virtual size_t GetLength() const = 0;
};