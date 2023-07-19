// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.IO:IOCompletionOverlapped;

export import Core.Std;
export import Core.System;

export class CORE_API IOCompletionOverlapped
{
	static constexpr size_t OverlapDefaultPaddingSize = 64;

	uint8 Padding[OverlapDefaultPaddingSize];
	IOCompletionOverlapped* Pointer;
	std::function<void(IOCompletionOverlapped*, size_t, int32)> Work;

public:
	IOCompletionOverlapped(std::function<void(IOCompletionOverlapped*, size_t, int32)> InWork);
	~IOCompletionOverlapped() noexcept = default;

	inline void Complete(size_t Resolved) { Work(this, Resolved, 0); }
	inline void Failed(int32 SystemCode) { Work(this, 0, SystemCode); }

public:
	inline void* ToOverlapped() const noexcept { return const_cast<uint8*>(Padding + 0); }
	static IOCompletionOverlapped* FromOverlapped(void* Overlapped) noexcept;
};