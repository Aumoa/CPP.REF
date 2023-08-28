// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

class CORE_API Thread
{
private:
	void* Impl = nullptr;

public:
	constexpr Thread() noexcept = default;

	constexpr bool IsValid() const noexcept { return Impl; }
	constexpr void* NativeHandle() const noexcept { return Impl; }
};