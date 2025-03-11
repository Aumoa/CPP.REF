// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <ranges>
#include "Linq/Details/linked_iterator_traits.h"
#include "System/AssertionMacros.h"

namespace Linq
{
	namespace details
	{
		template<std::input_iterator T1, std::input_iterator T2> requires
			requires { typename linked_iterator_traits<T1, T2>::value_type; }
		struct linked_iterator : public linked_iterator_traits<T1, T2>
		{
		private:
			using base = linked_iterator_traits<T1, T2>;

		private:
			T1 left_cur;
			T1 left_end;
			T2 right_cur;
			T2 right_end;

		public:
			using value_type = base::value_type;
			using difference_type = base::difference_type;
			using pointer = base::pointer;
			using reference = base::reference;

		public:
			constexpr linked_iterator() noexcept
			{
			}

			constexpr linked_iterator(T1 left_cur, T1 left_end, T2 right_cur, T2 right_end) noexcept
				: left_cur(std::move(left_cur))
				, left_end(std::move(left_end))
				, right_cur(std::move(right_cur))
				, right_end(std::move(right_end))
			{
			}

			constexpr linked_iterator(const linked_iterator&) noexcept(noexcept(left_cur = left_cur) && noexcept(right_cur = right_cur)) = default;
			constexpr linked_iterator(linked_iterator&&) noexcept = default;

			constexpr const reference operator *() const noexcept requires base::is_same
			{
				if (is_left_end())
				{
					return *right_cur;
				}
				else
				{
					return *left_cur;
				}
			}

			constexpr value_type operator *() const noexcept requires (!base::is_same)
			{
				if (is_left_end())
				{
					return *right_cur;
				}
				else
				{
					return *left_cur;
				}
			}

			constexpr linked_iterator operator ++(int) noexcept
			{
				if (is_left_end())
				{
					auto tmp = right_cur;
					std::advance(right_cur, 1);
					return linked_iterator(left_cur, left_end, tmp, right_end);
				}
				else
				{
					auto tmp = left_cur;
					std::advance(left_cur, 1);
					return linked_iterator(tmp, left_end, right_cur, right_end);
				}
			}

			constexpr linked_iterator& operator ++() noexcept
			{
				if (is_left_end())
				{
					std::advance(right_cur, 1);
					return *this;
				}
				else
				{
					std::advance(left_cur, 1);
					return *this;
				}
			}

			constexpr bool operator ==(const linked_iterator& rhs) const noexcept
			{
				check(left_end == rhs.left_end);
				check(right_end == rhs.right_end);
				return left_cur == rhs.left_cur
					&& right_cur == rhs.right_cur;
			}

			constexpr bool operator !=(const linked_iterator& rhs) const noexcept
			{
				check(left_end == rhs.left_end);
				check(right_end == rhs.right_end);
				return left_cur != rhs.left_cur
					|| right_cur != rhs.right_cur;
			}

			constexpr linked_iterator& operator =(const linked_iterator& rhs) noexcept(noexcept(left_cur = left_cur) && noexcept(right_cur = right_cur))
			{
				left_cur = rhs.left_cur;
				left_end = rhs.left_end;
				right_cur = rhs.right_cur;
				right_end = rhs.right_end;
				return *this;
			}

			constexpr linked_iterator& operator =(linked_iterator&& rhs) noexcept
			{
				left_cur = std::move(rhs.left_cur);
				left_end = std::move(rhs.left_end);
				right_cur = std::move(rhs.right_cur);
				right_end = std::move(rhs.right_end);
				return *this;
			}

		private:
			constexpr bool is_left_end() const noexcept
			{
				return left_cur == left_end;
			}
		};
	}
}