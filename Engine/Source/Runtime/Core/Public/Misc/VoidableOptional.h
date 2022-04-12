// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "CoreAssert.h"

namespace libty::inline Core
{
	template<class T>
	class VoidableOptional
	{
		bool _HasValue;
		int8 _Buf[sizeof(T)];

	public:
		VoidableOptional()
			: _HasValue(false)
		{
		}

		VoidableOptional(std::nullptr_t)
			: _HasValue(false)
		{
		}

		template<class U>
		VoidableOptional(U&& value)
			: _HasValue(true)
		{
			new(_Buf) T(std::forward<U>(value));
		}

		VoidableOptional(const VoidableOptional& rhs)
			: _HasValue(rhs._HasValue)
		{
			if (rhs._HasValue)
			{
				new(_Buf) T(rhs.GetValue());
			}
		}

		VoidableOptional(VoidableOptional&& rhs) noexcept
			: _HasValue(rhs._HasValue)
		{
			if (rhs._HasValue)
			{
				new(_Buf) T(std::move(rhs._Buf));
				rhs._HasValue = false;
			}
		}

		VoidableOptional& Reset()
		{
			if (_HasValue)
			{
				GetValue().~T();
				_HasValue = false;
			}
			return *this;
		}

		template<class U>
		VoidableOptional& SetValue(U&& value)
		{
			Reset();
			new(_Buf) T(std::forward<U>(value));
			_HasValue = true;
			return *this;
		}

		template<class... U>
		VoidableOptional& Emplace(U&&... args)
		{
			Reset();
			new(_Buf) T(std::forward<U>(args)...);
			_HasValue = true;
			return *this;
		}

		bool HasValue() const noexcept
		{
			return _HasValue;
		}

		T& GetValue()
		{
			check(_HasValue);
			return *(T*)_Buf;
		}

		const T& GetValue() const
		{
			check(_HasValue);
			return *(const T*)_Buf;
		}

		bool operator ==(const VoidableOptional& rhs) const noexcept(noexcept(GetValue() == rhs.GetValue()))
		{
			if (_HasValue != rhs._HasValue)
			{
				return false;
			}
			else if (!_HasValue)
			{
				return true;
			}
			else
			{
				return GetValue() == rhs.GetValue();
			}
		}

		T& operator *()
		{
			return GetValue();
		}

		const T& operator *() const
		{
			return GetValue();
		}

		T* operator ->()
		{
			return &GetValue();
		}

		const T* operator ->() const
		{
			return &GetValue();
		}
	};

	template<class T>
	VoidableOptional(T&&)->VoidableOptional<std::remove_const_t<std::remove_reference_t<T>>>;

	template<>
	class VoidableOptional<void>
	{
		bool _HasValue;

	public:
		VoidableOptional()
			: _HasValue(false)
		{
		}

		VoidableOptional(std::nullopt_t)
			: _HasValue(false)
		{
		}

		VoidableOptional(const VoidableOptional& rhs)
			: _HasValue(rhs._HasValue)
		{
		}

		VoidableOptional(VoidableOptional&& rhs) noexcept
			: _HasValue(rhs._HasValue)
		{
			rhs._HasValue = false;
		}

		VoidableOptional& Reset()
		{
			_HasValue = false;
			return *this;
		}

		VoidableOptional& SetValue()
		{
			_HasValue = true;
			return *this;
		}

		VoidableOptional& Emplace()
		{
			_HasValue = true;
			return *this;
		}

		bool HasValue() const noexcept
		{
			return _HasValue;
		}

		void GetValue() const
		{
			check(_HasValue);
		}

		bool operator ==(const VoidableOptional& rhs) const
		{
			return _HasValue == rhs._HasValue;
		}
	};
}