// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAssert.h"
#include "CoreObject/ObjectReference.h"

class AsyncValidator
{
	friend class Object;

private:
	ObjectReference* Ref = nullptr;
	bool Self = false;

private:
	inline AsyncValidator(ObjectReference* Ref, bool Self = false) noexcept
		: Ref(Ref)
		, Self(Self)
	{
		Ref->IncrWeak();
	}

public:
	inline AsyncValidator() noexcept
	{
	}

	inline AsyncValidator(const AsyncValidator& R)
		: Ref(R.Ref)
		, Self(R.Self)
	{
		if (Ref)
		{
			Ref->IncrWeak();
		}
	}

	inline AsyncValidator(AsyncValidator&& R)
		: Ref(R.Ref)
		, Self(R.Self)
	{
		R.Ref = nullptr;
		R.Self = false;
	}

	inline ~AsyncValidator() noexcept
	{
		if (Ref)
		{
			Ref->DecrWeak();
			Ref = nullptr;
		}
	}

	inline bool IsValid() const noexcept
	{
		return Ref && !Ref->bDisposed;
	}

	inline bool IsSelf() const noexcept
	{
		return Self;
	}

	inline void SetInvalid() noexcept
	{
		check(Self && IsValid());
		Ref->bDisposed = true;
	}

	inline AsyncValidator& operator =(const AsyncValidator& R)
	{
		if (Ref)
		{
			Ref->DecrWeak();
		}

		Ref = R.Ref;
		Self = R.Self;

		if (Ref)
		{
			Ref->IncrWeak();
		}

		return *this;
	}

	inline AsyncValidator& operator =(AsyncValidator&& R)
	{
		if (Ref)
		{
			Ref->DecrWeak();
		}

		Ref = R.Ref;
		Self = R.Self;
		R.Ref = nullptr;
		R.Self = false;

		return *this;
	}

	inline operator bool() const noexcept
	{
		return IsValid();
	}

	inline static AsyncValidator New()
	{
		return AsyncValidator(new ObjectReference(), true);
	}
};