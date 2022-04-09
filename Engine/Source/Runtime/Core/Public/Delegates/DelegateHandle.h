// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "ScopedDelegateHolder.h"

namespace libty::inline Core
{
	class DelegateHandle
	{
		template<class>
		friend class MulticastDelegate;

	private:
		ScopedDelegateHolder _delegate;

	public:
		DelegateHandle() = default;
		DelegateHandle(const DelegateHandle&) = default;
		DelegateHandle(DelegateHandle&&) = default;

		DelegateHandle(const ScopedDelegateHolder& delegate)
			: _delegate(delegate)
		{
		}

		DelegateHandle(ScopedDelegateHolder&& delegate)
			: _delegate(std::move(delegate))
		{
		}

		bool IsValid() const
		{
			return _delegate.IsValid();
		}

		void Reset()
		{
			if (IsValid())
			{
				_delegate->Remove(*this);
				_delegate.Reset();
			}
		}

		auto operator <=>(const DelegateHandle& rhs) const { return _delegate <=> rhs._delegate; }
		bool operator ==(const DelegateHandle& rhs) const { return _delegate == rhs._delegate; }

		DelegateHandle& operator =(const DelegateHandle&) = default;
		DelegateHandle& operator =(DelegateHandle&&) = default;
	};
}