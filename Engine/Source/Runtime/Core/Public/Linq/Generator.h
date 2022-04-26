// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "CoreConcepts.h"
#include <coroutine>

namespace libty::inline Core::Linq
{
	template<class T>
	class Generator
	{
	public:
		struct promise_type
		{
			std::optional<T> _value;
			std::exception_ptr _exception;

			Generator<T> get_return_object() noexcept
			{
				return Generator<T>(*this);
			}
			
			template<class U>
			auto yield_value(U&& value) noexcept(noexcept(_value = std::forward<U>(value)))
			{
				_value = std::forward<U>(value);
				return std::suspend_always();
			}

			auto yield_value(const T& value) noexcept(noexcept(_value = value))
			{
				_value = value;
				return std::suspend_always();
			}

			void unhandled_exception() noexcept
			{
				_exception = std::current_exception();
			}

			auto initial_suspend() const noexcept
			{
				return std::suspend_always();
			}

			auto final_suspend() const noexcept
			{
				return std::suspend_always();
			}

			void return_void() noexcept
			{
			}
		};

	private:
		std::coroutine_handle<promise_type> _coro;

	public:
		Generator() noexcept
		{
		}

		Generator(promise_type& prom) noexcept
			: _coro(std::coroutine_handle<promise_type>::from_promise(prom))
		{
		}

		~Generator() noexcept
		{
			if (_coro)
			{
				_coro.destroy();
			}
		}

		bool MoveNext()
		{
			this->_Throw_if();

			_coro.resume();
			return !_coro.done();
		}
		
		const T& Current() const
		{
			this->_Throw_if();
			return this->_Prom()->_value.value();
		}

	private:
		inline void _Throw_if() const
		{
			if (this->_Prom()->_exception)
			{
				std::rethrow_exception(this->_Prom()->_exception);
			}
		}

		inline promise_type* _Prom() const noexcept
		{
			return &_coro.promise();
		}
	};
}