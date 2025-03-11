// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "System/DelegateObjectValidator.h"
#include "System/VoidableOptional.h"
#include "System/ScopedRemover.h"
#include "Platform/PlatformMacros.h"
#include <concepts>
#include <functional>

template<class T>
class Delegate
{
	static_assert(std::same_as<T, std::function<void>>, "invalid template argument.");
};

template<class TRet, class... TArgs> requires requires { std::declval<std::function<TRet(TArgs...)>>(); }
class Delegate<TRet(TArgs...)>
{
public:
	using function_t = std::function<TRet(TArgs...)>;

private:
	function_t Impl;
	std::function<bool()> WeakCheck;
	std::function<bool(TRet*, std::function<TRet()>)> InvokeLock;

public:
	inline Delegate()
	{
	}

	inline bool IsValid() const noexcept
	{
		return IsBound() && IsLived();
	}

	inline bool IsBound() const noexcept
	{
		return (bool)Impl;
	}

	inline bool IsLived() const noexcept
	{
		return (WeakCheck ? WeakCheck() : true);
	}

#define DELEGATE_INVOKE_FUNCTION std::invoke(Impl, std::forward<UArgs>(InArgs)...)

	template<class... UArgs> requires std::invocable<function_t, UArgs...>
	inline auto Invoke(UArgs&&... InArgs) const noexcept(noexcept(DELEGATE_INVOKE_FUNCTION))
	{
		return DELEGATE_INVOKE_FUNCTION;
	}

	template<class... UArgs> requires std::invocable<function_t, UArgs...>
	inline VoidableOptional<TRet> SafeInvoke(UArgs&&... InArgs) const noexcept(noexcept(DELEGATE_INVOKE_FUNCTION))
	{
		if (IsBound())
		{
			if (WeakCheck)
			{
				if (InvokeLock)
				{
					if constexpr (std::is_void_v<TRet>)
					{
						if (InvokeLock(nullptr, [&]() { return Impl(std::forward<UArgs>(InArgs)...); }))
						{
							return VoidableOptional<void>::Completed();
						}
						else
						{
							return VoidableOptional<void>::Failure();
						}
					}
					else
					{
						char ResultBuf[sizeof(TRet)];
						if (char ResultBuf[sizeof(TRet)]; InvokeLock(reinterpret_cast<TRet*>(ResultBuf), [&]() { return Impl(std::forward<UArgs>(InArgs)...); }))
						{
							ScopedRemover<TRet> Remover(ResultBuf);
							return *reinterpret_cast<TRet*>(ResultBuf);
						}
						else
						{
							return std::nullopt;
						}
					}
				}
				else if (WeakCheck())
				{
					if constexpr (std::is_void_v<TRet>)
					{
						DELEGATE_INVOKE_FUNCTION;
						return VoidableOptional<void>::Completed();
					}
					else
					{
						return DELEGATE_INVOKE_FUNCTION;
					}
				}
				else
				{
					if constexpr (std::is_void_v<TRet>)
					{
						return VoidableOptional<void>::Failure();
					}
					else
					{
						return std::nullopt;
					}
				}
			}
			else
			{
				if constexpr (std::is_void_v<TRet>)
				{
					DELEGATE_INVOKE_FUNCTION;
					return VoidableOptional<void>::Completed();
				}
				else
				{
					return DELEGATE_INVOKE_FUNCTION;
				}
			}
		}
		else
		{
			if constexpr (std::is_void_v<TRet>)
			{
				return VoidableOptional<void>::Failure();
			}
			else
			{
				return std::nullopt;
			}
		}
	}

	template<class... UArgs> requires std::invocable<function_t, UArgs...>
	FORCEINLINE auto operator ()(UArgs&&... InArgs) const noexcept(noexcept(DELEGATE_INVOKE_FUNCTION))
	{
		return Invoke(std::forward<UArgs>(InArgs)...);
	}

#undef DELEGATE_INVOKE_FUNCTION

public:
	template<class U> requires std::constructible_from<function_t, U>
	static Delegate CreateLambda(U&& Val)
	{
		Delegate Inst;
		Inst.Impl = function_t(std::forward<U>(Val));
		return Inst;
	}

	static Delegate CreateStatic(TRet(*Val)(TArgs...))
	{
		Delegate Inst;
		Inst.Impl = function_t(Val);
		return Inst;
	}

	template<class UObject, class URef = std::remove_pointer_t<std::remove_reference_t<UObject>>>
	static Delegate CreateRaw(UObject&& Obj, TRet(URef::*Val)(TArgs...))
	{
		Delegate Inst;
		if constexpr (std::is_pointer_v<std::remove_reference_t<UObject>>)
		{
			Inst.Impl = [Obj, Val](TArgs&&... InArgs) { return (Obj->*Val)(std::forward<TArgs>(InArgs)...); };
		}
		else
		{
			Inst.Impl = [Obj = &Obj, Val](TArgs&&... InArgs) { return (Obj->*Val)(std::forward<TArgs>(InArgs)...); };
		}
		return Inst;
	}

	template<class UObject, class ULambda> requires
		std::constructible_from<function_t, ULambda> &&
		DelegateObjectValidator<UObject>::bSpecialized
	static Delegate CreateWeakLambda(const UObject& Obj, ULambda&& Val)
	{
		using Validator = DelegateObjectValidator<UObject>;
		Delegate Inst;
		Inst.Impl = function_t(std::forward<ULambda>(Val));
		Inst.WeakCheck = [Weak = Validator::GetWeak(Obj)]() { return Validator::IsValid(Weak); };
		Inst.InvokeLock = GenerateLock<Validator>(Obj);
		if constexpr (Validator::bCanLock)
		{
			Inst.InvokeLock = [Weak = Validator::GetWeak(Obj)](TRet& InvokeResult, std::function<TRet()> Val)
			{
				auto Locked = Validator::GetLock(Weak);
				if (Validator::IsValid(Locked))
				{
					new(&InvokeResult) TRet(Val());
					return true;
				}
				else
				{
					return false;
				}
			};
		}
		return Inst;
	}

	template<class UObject, class URef = std::remove_pointer_t<std::remove_reference_t<UObject>>> requires
		DelegateObjectValidator<UObject>::bSpecialized
	static Delegate CreateObject(UObject&& Obj, TRet(URef::*Val)(TArgs...))
	{
		using Validator = DelegateObjectValidator<UObject>;
		Delegate Inst;
		if constexpr (std::is_pointer_v<std::remove_reference_t<UObject>>)
		{
			Inst.Impl = [Obj, Val](TArgs&&... InArgs) { return (Obj->*Val)(std::forward<TArgs>(InArgs)...); };
		}
		else
		{
			Inst.Impl = [Obj = &Obj, Val](TArgs&&... InArgs) { return (Obj->*Val)(std::forward<TArgs>(InArgs)...); };
		}
		Inst.WeakCheck = [Weak = Validator::GetWeak(Obj)]() { return Validator::IsValid(Weak); };
		Inst.InvokeLock = GenerateLock<Validator>(Obj);
		return Inst;
	}

private:
	template<class TValidator, class UObject>
	static std::function<bool(TRet*, std::function<TRet()>)> GenerateLock(UObject&& Obj)
	{
		if constexpr (TValidator::bCanLock)
		{
			return [Weak = TValidator::GetWeak(Obj)](TRet* InvokeResult, std::function<TRet()> Val)
			{
				auto Locked = TValidator::GetLock(Weak);
				if (TValidator::IsValid(Locked))
				{
					if constexpr (std::is_void_v<TRet> == false)
					{
						new(InvokeResult) TRet(Val());
					}
					else
					{
						Val();
					}
					return true;
				}
				else
				{
					return false;
				}
			};
		}
		else
		{
			return nullptr;
		}
	}
};