// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "TNumericsBasicOperator.h"

template<class T, class... TItems>
template<class TItem>
inline constexpr bool TNumericsBasicOperator<T, TItems...>::LessOp::Op1(const TItem& lh, const TItem& rh)
{
	return lh < rh;
}

template<class T, class... TItems>
template<class TItem>
inline constexpr bool TNumericsBasicOperator<T, TItems...>::LessOp::Op2(const TItem& lh, const TItem& rh)
{
	return lh > rh;
}

template<class T, class... TItems>
template<class TItem>
inline constexpr bool TNumericsBasicOperator<T, TItems...>::LessOp::Op3(const TItem& lh, const TItem& rh)
{
	return lh < rh;
}

template<class T, class... TItems>
template<class TItem>
inline constexpr bool TNumericsBasicOperator<T, TItems...>::LessEqualsOp::Op3(const TItem& lh, const TItem& rh)
{
	return lh <= rh;
}

template<class T, class... TItems>
template<class TItem>
inline constexpr bool TNumericsBasicOperator<T, TItems...>::GreaterOp::Op1(const TItem& lh, const TItem& rh)
{
	return lh > rh;
}

template<class T, class... TItems>
template<class TItem>
inline constexpr bool TNumericsBasicOperator<T, TItems...>::GreaterOp::Op2(const TItem& lh, const TItem& rh)
{
	return lh < rh;
}

template<class T, class... TItems>
template<class TItem>
inline constexpr bool TNumericsBasicOperator<T, TItems...>::GreaterOp::Op3(const TItem& lh, const TItem& rh)
{
	return lh > rh;
}

template<class T, class... TItems>
template<class TItem>
inline constexpr bool TNumericsBasicOperator<T, TItems...>::GreaterEqualsOp::Op3(const TItem& lh, const TItem& rh)
{
	return lh >= rh;
}

template<class T, class... TItems>
inline bool TNumericsBasicOperator<T, TItems...>::operator ==(const T& rh) const
{
	return operator_equals(rh, std::make_index_sequence<sizeof...(TItems)>{ });
}

template<class T, class... TItems>
inline bool TNumericsBasicOperator<T, TItems...>::operator !=(const T& rh) const
{
	return operator_notequals(rh, std::make_index_sequence<sizeof...(TItems)>{ });
}

template<class T, class... TItems>
inline bool TNumericsBasicOperator<T, TItems...>::operator < (const T& rh) const
{
	return operator_compare<0, LessOp>(rh);
}

template<class T, class... TItems>
inline bool TNumericsBasicOperator<T, TItems...>::operator <=(const T& rh) const
{
	return operator_compare<0, LessEqualsOp>(rh);
}

template<class T, class... TItems>
inline bool TNumericsBasicOperator<T, TItems...>::operator > (const T& rh) const
{
	return operator_compare<0, GreaterOp>(rh);
}

template<class T, class... TItems>
inline bool TNumericsBasicOperator<T, TItems...>::operator >=(const T& rh) const
{
	return operator_compare<0, GreaterEqualsOp>(rh);
}

template<class T, class... TItems>
inline std::weak_ordering TNumericsBasicOperator<T, TItems...>::operator <=>(const T& rh) const
{
	return operator_ordering<0>(rh);
}

template<class T, class... TItems>
template<size_t N>
inline auto TNumericsBasicOperator<T, TItems...>::GetItemByIndex() const -> const typename IndexTypeTraits<N>::TItem&
{
	using _Mytraits = IndexTypeTraits<N>;
	using _Myt = typename _Mytraits::TItem;
	return *GetPointer<_Myt>(_Mytraits::ByteOffset - sizeof(_Myt));
}

template<class T, class... TItems>
template<class TCast>
inline const TCast* TNumericsBasicOperator<T, TItems...>::GetPointer(size_t N) const
{
	return reinterpret_cast<const TCast*>(reinterpret_cast<const uint8*>(this) + N);
}

template<class T, class... TItems>
template<size_t... Sq>
inline bool TNumericsBasicOperator<T, TItems...>::operator_equals(const T& rh, std::index_sequence<Sq...>&&) const
{
	return ((GetItemByIndex<Sq>() == rh.GetItemByIndex<Sq>()) && ...);
}

template<class T, class... TItems>
template<size_t... Sq>
inline bool TNumericsBasicOperator<T, TItems...>::operator_notequals(const T& rh, std::index_sequence<Sq...>&&) const
{
	return ((GetItemByIndex<Sq>() != rh.GetItemByIndex<Sq>()) || ...);
}

template<class T, class... TItems>
template<size_t N, class TOp>
inline bool TNumericsBasicOperator<T, TItems...>::operator_compare(const T& rh) const
{
	if constexpr (N == sizeof...(TItems) - 1)
	{
		if (TOp::Op3(GetItemByIndex<N>(), rh.GetItemByIndex<N>()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (TOp::Op1(GetItemByIndex<N>(), rh.GetItemByIndex<N>()))
		{
			return true;
		}
		else if (TOp::Op2(GetItemByIndex<N>(), rh.GetItemByIndex<N>()))
		{
			return false;
		}
		else
		{
			return operator_compare<N + 1, TOp>(rh);
		}
	}
}

template<class T, class... TItems>
template<size_t N>
inline std::weak_ordering TNumericsBasicOperator<T, TItems...>::operator_ordering(const T& rh) const
{
	if constexpr (N == sizeof...(TItems))
	{
		return std::weak_ordering::equivalent;
	}
	else
	{
		if (GetItemByIndex<N>() < rh.GetItemByIndex<N>())
		{
			return std::weak_ordering::less;
		}
		else if (GetItemByIndex<N>() > rh.GetItemByIndex<N>())
		{
			return std::weak_ordering::greater;
		}
		else
		{
			return operator_ordering<N + 1>(rh);
		}
	}
}