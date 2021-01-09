// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <type_traits>
#include <compare>

template<class T, class... TItems>
struct TNumericsBasicOperator
{
private:
	template<size_t N, class TSelect, class... TItems>
	struct TIndexTypeTraits
	{
		using TItem = typename TIndexTypeTraits<N - 1, TItems...>::TItem;
		static constexpr const size_t ByteOffset = sizeof(TSelect) + TIndexTypeTraits<N - 1, TItems...>::ByteOffset;
	};

	template<class TSelect, class... TItems>
	struct TIndexTypeTraits<0, TSelect, TItems...>
	{
		using TItem = TSelect;
		static constexpr const size_t ByteOffset = sizeof(TSelect);
	};

	template<size_t N>
	using IndexTypeTraits = TIndexTypeTraits<N, TItems...>;

	struct LessOp
	{
		template<class TItem>
		inline constexpr static bool Op1(const TItem& lh, const TItem& rh);
		template<class TItem>
		inline constexpr static bool Op2(const TItem& lh, const TItem& rh);
		template<class TItem>
		inline constexpr static bool Op3(const TItem& lh, const TItem& rh);
	};

	struct LessEqualsOp : public LessOp
	{
		template<class TItem>
		inline constexpr static bool Op3(const TItem& lh, const TItem& rh);
	};

	struct GreaterOp
	{
		template<class TItem>
		inline constexpr static bool Op1(const TItem& lh, const TItem& rh);
		template<class TItem>
		inline constexpr static bool Op2(const TItem& lh, const TItem& rh);
		template<class TItem>
		inline constexpr static bool Op3(const TItem& lh, const TItem& rh);
	};

	struct GreaterEqualsOp : public GreaterOp
	{
		template<class TItem>
		inline constexpr static bool Op3(const TItem& lh, const TItem& rh);
	};

public:
	inline bool operator ==(const T& rh) const;
	inline bool operator !=(const T& rh) const;
	inline bool operator < (const T& rh) const;
	inline bool operator <=(const T& rh) const;
	inline bool operator > (const T& rh) const;
	inline bool operator >=(const T& rh) const;
	inline std::weak_ordering operator <=>(const T& rh) const;

private:
	template<size_t N>
	inline auto GetItemByIndex() const -> const typename IndexTypeTraits<N>::TItem&;
	template<class TCast>
	inline const TCast* GetPointer(size_t N) const;

	template<size_t... Sq>
	inline bool operator_equals(const T& rh, std::index_sequence<Sq...>&&) const;
	template<size_t... Sq>
	inline bool operator_notequals(const T& rh, std::index_sequence<Sq...>&&) const;
	template<size_t N, class TOp>
	inline bool operator_compare(const T& rh) const;
	template<size_t N>
	inline std::weak_ordering operator_ordering(const T& rh) const;
};

#include "TNumericsBasicOperator.inl"