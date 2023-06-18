// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.Diagnostics:StackTrace;

export import :StackFrame;
export import <thread>;
export import <map>;
export import <mutex>;

export class CORE_API StackTrace
{
	struct Implementation;

private:
	std::vector<StackFrame> Frames;

public:
	StackTrace() noexcept;

	std::span<const StackFrame> GetFrames() const noexcept;
	size_t GetFrameCount() const noexcept;
	String ToString() const noexcept;

public:
	static StackTrace Current() noexcept;
	static StackTrace FromThread(std::thread& Thrd) noexcept;
	static StackTrace FromException(void* lpExceptionPointers) noexcept;
};