// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <coroutine>
#include <exception>
#include <optional>

namespace Linq
{
	template<class T>
	class Generator
	{
		Generator(const Generator&) = delete;
		class Enumerator;
		template<class U>
		friend class Generator;

	public:
		class promise_type
		{
			friend Enumerator;

			std::optional<T> _value;
			std::exception_ptr _exception;

		public:
			inline promise_type() noexcept
			{
			}

			inline Generator<T> get_return_object() noexcept
			{
				return Generator<T>(*this);
			}

			template<class U>
			inline auto yield_value(U&& value) noexcept(noexcept(_Emplace_value(std::forward<U>(value))))
			{
				return this->_Emplace_value(std::forward<U>(value));
			}

			inline auto yield_value(const T& value) noexcept(noexcept(_Emplace_value(value)))
			{
				return this->_Emplace_value(value);
			}

			inline void unhandled_exception() noexcept
			{
				_exception = std::current_exception();
			}

			inline auto initial_suspend() const noexcept
			{
				return std::suspend_never();
			}

			inline auto final_suspend() const noexcept
			{
				return std::suspend_always();
			}

			inline void return_void() noexcept
			{
			}

		private:
			template<class U>
			auto _Emplace_value(U&& value) noexcept(noexcept(_value = std::forward<U>(value)))
			{
				_value = std::forward<U>(value);
				return std::suspend_always();
			}

			const T& _Get_current() const
			{
				if (!_value.has_value())
				{
					Generator<void>::_Xthrow_value_null();
				}
				return *_value;
			}
		};

	public:
		class Enumerator
		{
			Enumerator(const Enumerator&) = delete;

		private:
			std::coroutine_handle<promise_type> _coro;

		public:
			inline Enumerator(std::coroutine_handle<promise_type> coro) noexcept
				: _coro(std::move(coro))
			{
			}

			inline const T& operator *() const noexcept
			{
				return _coro.promise()._Get_current();
			}

			inline Enumerator& operator ++()
			{
				if (_coro.done())
				{
					Generator<void>::_Xthrow_coro_done();
				}

				_coro.resume();
				return *this;
			}

			inline bool IsDone() const noexcept
			{
				return _coro.done();
			}

		public:
			inline bool operator !=(std::nullptr_t) const noexcept
			{
				return !IsDone();
			}
		};

	private:
		std::coroutine_handle<promise_type> _coro;

	public:
		inline Generator() noexcept
		{
		}

		inline Generator(promise_type& promise) noexcept
			: _coro(decltype(_coro)::from_promise(promise))
		{
		}

		inline Generator(Generator&& rhs) noexcept
			: _coro(std::move(rhs._coro))
		{
		}

		inline ~Generator() noexcept
		{
			if (_coro)
			{
				_coro.destroy();
			}
		}

		Enumerator begin() noexcept
		{
			if (!_coro)
			{
				Generator<void>::_Xthrow_already_start();
			}

			return Enumerator(std::move(_coro));
		}

		std::nullptr_t end() noexcept
		{
			return nullptr;
		}

		inline bool IsDone() const noexcept
		{
			return _coro.done();
		}

	public:
		Generator& operator =(Generator&& rhs) noexcept
		{
			_coro = std::move(rhs._coro);
			return *this;
		}

	private:
		[[noreturn]] static void _Xthrow_already_start();
		[[noreturn]] static void _Xthrow_value_null();
		[[noreturn]] static void _Xthrow_coro_done();
	};

	template<>
	[[noreturn]] CORE_API static void Generator<void>::_Xthrow_already_start();
	template<>
	[[noreturn]] CORE_API static void Generator<void>::_Xthrow_value_null();
	template<>
	[[noreturn]] CORE_API static void Generator<void>::_Xthrow_coro_done();
}