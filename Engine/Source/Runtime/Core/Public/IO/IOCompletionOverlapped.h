// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreTypes/IntegralTypes.h"
#include "Misc/PlatformMisc.h"
#include <vector>
#include <exception>

class CORE_API IOCompletionOverlapped
{
	static constexpr size_t OverlapDefaultPaddingSize = 64;

	uint8 _pad[OverlapDefaultPaddingSize];
	IOCompletionOverlapped* _ptr;
	std::function<void(size_t, int32)> _work;

public:
	IOCompletionOverlapped(std::function<void(size_t, int32)> work);
	~IOCompletionOverlapped() noexcept = default;

	inline void Complete(size_t resolved) { _work(resolved, 0); }
	inline void Failed(int32 systemCode) { _work(0, systemCode); }

public:
	inline void* ToOverlapped() const noexcept { return const_cast<uint8*>(_pad + 0); }
	static IOCompletionOverlapped* FromOverlapped(void* overlapped) noexcept;
};