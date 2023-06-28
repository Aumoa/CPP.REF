// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.System:VoidableOptional;

export import Core.Std;
export import :IntegralTypes;

export
template<class T>
class VoidableOptional
{
	bool bHasValue;
	int8 Buf[sizeof(T)];

public:
	VoidableOptional()
		: bHasValue(false)
	{
	}

	VoidableOptional(std::nullptr_t)
		: bHasValue(false)
	{
	}

	template<class U>
	VoidableOptional(U&& InValue)
		: bHasValue(true)
	{
		new(Buf) T(std::forward<U>(InValue));
	}

	VoidableOptional(const VoidableOptional& Rhs)
		: bHasValue(Rhs.bHasValue)
	{
		if (Rhs.bHasValue)
		{
			new(Buf) T(Rhs.GetValue());
		}
	}

	VoidableOptional(VoidableOptional&& Rhs) noexcept
		: bHasValue(Rhs.bHasValue)
	{
		if (Rhs.bHasValue)
		{
			new(Buf) T(std::move(Rhs.Buf));
			Rhs.bHasValue = false;
		}
	}

	~VoidableOptional()
	{
		Reset();
	}

	VoidableOptional& Reset()
	{
		if (bHasValue)
		{
			GetValue().~T();
			bHasValue = false;
		}
		return *this;
	}

	template<class U>
	VoidableOptional& SetValue(U&& InValue)
	{
		Reset();
		new(Buf) T(std::forward<U>(InValue));
		bHasValue = true;
		return *this;
	}

	template<class... U>
	VoidableOptional& Emplace(U&&... InArgs)
	{
		Reset();
		new(Buf) T(std::forward<U>(InArgs)...);
		bHasValue = true;
		return *this;
	}

	bool HasValue() const noexcept
	{
		return bHasValue;
	}

	T& GetValue()
	{
		check(bHasValue);
		return *(T*)Buf;
	}

	const T& GetValue() const
	{
		check(bHasValue);
		return *(const T*)Buf;
	}

	bool operator ==(const VoidableOptional& Rhs) const noexcept(noexcept(GetValue() == Rhs.GetValue()))
	{
		if (bHasValue != Rhs.bHasValue)
		{
			return false;
		}
		else if (!bHasValue)
		{
			return true;
		}
		else
		{
			return GetValue() == Rhs.GetValue();
		}
	}

	T& operator *()
	{
		return GetValue();
	}

	VoidableOptional& operator =(const VoidableOptional& Rhs)
	{
		Reset();
		bHasValue = Rhs.bHasValue;
		if (Rhs.bHasValue)
		{
			new(Buf) T(Rhs.GetValue());
		}
		return *this;
	}

	VoidableOptional& operator =(VoidableOptional&& Rhs) noexcept
	{
		Reset();
		bHasValue = Rhs.bHasValue;
		if (Rhs.bHasValue)
		{
			new(Buf) T(std::move(Rhs.Buf));
			Rhs.bHasValue = false;
		}
		return *this;
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
VoidableOptional(T&&) -> VoidableOptional<std::remove_const_t<std::remove_reference_t<T>>>;

template<>
class VoidableOptional<void>
{
	bool bHasValue;

public:
	VoidableOptional()
		: bHasValue(false)
	{
	}

	VoidableOptional(std::nullopt_t)
		: bHasValue(false)
	{
	}

	VoidableOptional(const VoidableOptional& Rhs)
		: bHasValue(Rhs.bHasValue)
	{
	}

	VoidableOptional(VoidableOptional&& Rhs) noexcept
		: bHasValue(Rhs.bHasValue)
	{
		Rhs.bHasValue = false;
	}

	VoidableOptional& Reset()
	{
		bHasValue = false;
		return *this;
	}

	VoidableOptional& SetValue()
	{
		bHasValue = true;
		return *this;
	}

	VoidableOptional& Emplace()
	{
		bHasValue = true;
		return *this;
	}

	bool HasValue() const noexcept
	{
		return bHasValue;
	}

	void GetValue() const
	{
		check(bHasValue);
	}

	bool operator ==(const VoidableOptional& Rhs) const
	{
		return bHasValue == Rhs.bHasValue;
	}
};