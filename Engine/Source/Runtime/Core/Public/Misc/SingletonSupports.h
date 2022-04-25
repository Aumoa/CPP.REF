// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Core
{
	template<class T>
	class SingletonSupports
	{
		static inline T* sInstance;

	public:
		SingletonSupports(T* instance)
		{
			checkf(sInstance == nullptr, "Singleton class already instanced.");
			sInstance = instance;
		}

		~SingletonSupports() noexcept
		{
			sInstance = nullptr;
		}

		[[nodiscard]] static T* Get() noexcept
		{
			return sInstance;
		}
	};
}