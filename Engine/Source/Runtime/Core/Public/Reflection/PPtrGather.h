// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <vector>

namespace Ayla
{
	class Object;
	template<class T>
	class PPtr;

	template<class T = void>
	struct PPtrGather
	{
		static_assert(std::same_as<T, void>);
	};

	template<>
	struct PPtrGather<void>
	{
	protected:
		PPtrGather() = default;

	public:
		PPtrGather(const PPtrGather&) = delete;
		virtual ~PPtrGather() noexcept = default;

		virtual void PullPPtrs(Object* pointer, size_t offset, std::vector<Object*>& output) = 0;

		PPtrGather& operator =(const PPtrGather&) = delete;

		template<class U>
		inline static PPtrGather<U>* Get()
		{
			static PPtrGather<U> s_Inst;
			return &s_Inst;
		}
	};
}