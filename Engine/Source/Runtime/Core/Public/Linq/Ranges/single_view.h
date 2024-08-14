// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <ranges>

namespace Linq::ranges
{
	template<class T>
	struct single_view : public std::ranges::view_interface<single_view<T>>
	{
	public:
		struct iterator
		{
		public:
			using value_type = std::remove_reference_t<T>;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;

		private:
			value_type* ptr;

		public:
			constexpr iterator() noexcept
			{
			}

			constexpr iterator(value_type* ptr) noexcept
				: ptr(ptr)
			{
			}

			constexpr iterator(const iterator&) noexcept = default;
			constexpr iterator(iterator&&) noexcept = default;

			constexpr const reference operator *() const noexcept
			{
				return *ptr;
			}

			constexpr iterator operator ++(int) noexcept
			{
				auto tmp = ptr;
				ptr = nullptr;
				return iterator(tmp);
			}

			constexpr iterator& operator ++() noexcept
			{
				ptr = nullptr;
				return *this;
			}

			constexpr bool operator ==(const iterator& rhs) const noexcept
			{
				return ptr == rhs.ptr;
			}

			constexpr bool operator !=(const iterator& rhs) const noexcept
			{
				return ptr != rhs.ptr;
			}

			constexpr iterator& operator =(const iterator& rhs) noexcept = default;
			constexpr iterator& operator =(iterator&& rhs) noexcept = default;
		};

	private:
		T value;

	public:
		template<class U>
		single_view(U&& value)
			: value(std::forward<U>(value))
		{
		}

		constexpr auto begin() noexcept { return iterator(&value); }
		constexpr auto end() noexcept { return iterator(nullptr); }
		constexpr auto begin() const noexcept { return iterator(&value); }
		constexpr auto end() const noexcept { return iterator(nullptr); }
		constexpr size_t size() const noexcept { return 1; }
	};

	template<class T>
	single_view(T&&) -> single_view<T>;
}