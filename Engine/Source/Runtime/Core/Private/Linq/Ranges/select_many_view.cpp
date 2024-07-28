// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:select_many_view;

export import :Std;

export namespace Linq::ranges
{
	template<
		std::ranges::input_range R,
		class T,
		class R_iterator = std::ranges::iterator_t<R>,
		class R_value = typename std::iterator_traits<R_iterator>::value_type,
		std::ranges::input_range T_view = std::invoke_result_t<T, R_value>
	>
	struct select_many_view : public std::ranges::view_interface<select_many_view<R, T, R_iterator, R_value, T_view>>
	{
		using T_iterator = std::ranges::iterator_t<const T_view>;

	private:
		R r;
		T t;

	public:
		struct iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = typename std::iterator_traits<T_iterator>::value_type;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;

		public:
			R_iterator layer_cur;
			R_iterator layer_end;
			T t;
			T_iterator cur;
			T_iterator end;

		public:
			constexpr iterator() noexcept
			{
			}

			constexpr iterator(R_iterator layer_cur, R_iterator layer_end, T t, T_iterator cur, T_iterator end) noexcept
				: layer_cur(std::move(layer_cur))
				, layer_end(std::move(layer_end))
				, t(std::move(t))
				, cur(std::move(cur))
				, end(std::move(end))
			{
			}

			constexpr iterator(const iterator&) noexcept = default;
			constexpr iterator(iterator&&) noexcept = default;

			constexpr auto operator *() const noexcept
			{
				return *cur;
			}

			constexpr iterator operator ++(int) noexcept
			{
				iterator tmp = *this;

				if (++cur == end)
				{
					++layer_cur;
					if (layer_cur != layer_end)
					{
						cur = std::ranges::begin(t(*layer_cur));
						end = std::ranges::end(t(*layer_cur));
					}
					else
					{
						cur = {};
						end = {};
					}
				}

				return tmp;
			}

			constexpr iterator& operator ++() noexcept
			{
				if (++cur == end)
				{
					++layer_cur;
					if (layer_cur != layer_end)
					{
						cur = std::ranges::begin(t(*layer_cur));
						end = std::ranges::end(t(*layer_cur));
					}
					else
					{
						cur = {};
						end = {};
					}
				}

				return *this;
			}

			constexpr bool operator ==(const iterator& rhs) const noexcept
			{
				return layer_cur == rhs.layer_cur
					&& layer_end == rhs.layer_end
					&& cur == rhs.cur
					&& end == rhs.end;
			}

			constexpr bool operator !=(const iterator& rhs) const noexcept
			{
				return layer_cur != rhs.layer_cur
					|| layer_end != rhs.layer_end
					|| cur != rhs.cur
					|| end != rhs.end;
			}

			constexpr iterator& operator =(const iterator& rhs) noexcept;
			constexpr iterator& operator =(iterator&& rhs) noexcept;
		};

	public:
		template<std::ranges::input_range U, class P>
		constexpr select_many_view(U&& u, P&& p) noexcept
			: r(std::forward<U>(u))
			, t(std::forward<P>(p))
		{
		}

		constexpr auto begin() noexcept
		{
			auto begin = std::ranges::begin(r);
			auto inner = t(*begin);
			return iterator(begin, std::ranges::end(r), t, std::ranges::begin(inner), std::ranges::end(inner));
		}

		constexpr auto end() noexcept
		{
			return iterator(std::ranges::end(r), std::ranges::end(r), t, {}, {});
		}

		constexpr auto begin() const noexcept
		{
			auto begin = std::ranges::begin(r);
			auto inner = t(*begin);
			return iterator(begin, std::ranges::end(r), t, std::ranges::begin(inner), std::ranges::end(inner));
		}

		constexpr auto end() const noexcept
		{
			return iterator(std::ranges::end(r), std::ranges::end(r), t, {}, {});
		}
	};

	template<
		std::ranges::input_range R,
		class T,
		class R_iterator = std::ranges::iterator_t<R>,
		class R_value = typename std::iterator_traits<R_iterator>::value_type,
		std::ranges::input_range T_view = std::invoke_result_t<T, R_value>
	>
	select_many_view(R&&, T&&) -> select_many_view<R, T>;
}