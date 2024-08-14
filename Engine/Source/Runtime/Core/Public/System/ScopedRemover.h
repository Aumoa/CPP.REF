// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

template<class T>
class ScopedRemover
{
private:
	char* Buf;
	bool bExpired = false;

public:
	constexpr ScopedRemover(char Buf[]) noexcept
		: Buf(Buf)
	{
	}

	constexpr ~ScopedRemover() noexcept
	{
		static_assert(noexcept(std::declval<T>().~T()), "destructor must be noexcept.");
		if (bExpired == false)
		{
			reinterpret_cast<T*>(Buf)->~T();
		}
	}

	constexpr void Expire() noexcept
	{
		bExpired = true;
	}
};