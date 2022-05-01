// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IMulticastDelegate.h"
#include "DelegateHandle.h"
#include "Threading/Spinlock.h"
#include <functional>
#include <map>
#include <atomic>
#include <concepts>
#include <memory>

namespace libty::inline Core
{
	template<class>
	class MulticastDelegate
	{
	};

	template<class... TArgs>
	class MulticastDelegate<void(TArgs...)> : public IMulticastDelegate
	{
		using TPayload = std::function<void(TArgs...)>;

		struct DelegateInstance
		{
			TPayload Body;
			std::shared_ptr<int64> Id;
			std::function<bool()> Holder;

			DelegateInstance(int64 id, TPayload payload)
				: Body(std::move(payload))
				, Id(std::make_shared<int64>(id))
			{
			}

			DelegateInstance(int64 id, TPayload payload, std::function<bool()> holder)
				: Body(std::move(payload))
				, Id(std::make_shared<int64>(id))
				, Holder(std::move(holder))
			{
			}

			template<class... TInvokeArgs>
			bool Invoke(TInvokeArgs&&... InArgs)
			{
				if (Holder && !Holder())
				{
					return false;
				}

				Body(std::forward<TInvokeArgs>(InArgs)...);
				return true;
			}
		};

		Spinlock _lock;
		std::map<int64, DelegateInstance> _payload;
		int64 _id = 0;

	public:
		MulticastDelegate() = default;

		template<class... TInvokeArgs>
		void Broadcast(TInvokeArgs&&... args)
		{
			std::vector<int64> compacts;
			std::vector<DelegateInstance> invokes;

			{
				auto lock = std::unique_lock<Spinlock>(_lock, Spinlock::Readonly);
				invokes.reserve(_payload.size());
				for (auto& instance : _payload)
				{
					invokes.emplace_back(instance.second);
				}
			}

			for (auto& invoke : invokes)
			{
				if (!invoke.Invoke(std::forward<TInvokeArgs>(args)...))
				{
					compacts.emplace_back(*invoke.Id);
				}
			}

			if (compacts.size())
			{
				std::unique_lock lock(_lock);
				for (auto& compact : compacts)
				{
					_payload.erase(compact);
				}
			}
		}

		void Clear()
		{
			std::unique_lock lock(_lock);
			_payload.clear();
			_id = 0;
		}

		template<class TFunctionBody>
		ScopedDelegateHolder AddRaw(TFunctionBody&& body) requires requires
		{
			{ TPayload(std::declval<TFunctionBody>()) };
		}
		{
			std::unique_lock lock(_lock);
			int64 myid = _id++;

			DelegateInstance instance(myid, std::move(body));
			ScopedDelegateHolder handle(this, instance.Id);
			_payload.emplace(myid, std::move(instance));
			return handle;
		}

		template<class U>
		ScopedDelegateHolder AddObject(U* object, void(U::* body)(TArgs...))
		{
			std::unique_lock lock(_lock);
			int64 myid = _id++;

			auto invoke = [object, body](TArgs&&... args)
			{
				(object->*body)(std::forward<TArgs>(args)...);
			};

			DelegateInstance instance(myid, invoke, GetHolderImpl(object));
			ScopedDelegateHolder handle(this, instance.Id);
			_payload.emplace(myid, std::move(instance));
			return handle;
		}

		virtual void Remove(DelegateHandle& handle) override
		{
			std::unique_lock lock(_lock);
			if (handle.IsValid())
			{
				_payload.erase(handle._delegate._id);
				handle._delegate.Reset();
			}
		}

		template<class TFunctionBody>
		auto operator +=(TFunctionBody&& body)
		{
			return AddRaw(std::forward<TFunctionBody>(body));
		}

	private:
		template<class U>
		static auto GetHolderImpl(U* object) requires requires
		{
			{ std::declval<U>().GetHolder()() } -> std::convertible_to<bool>;
		}
		{
			return object->GetHolder();
		}

		template<class U>
		static auto GetHolderImpl(U* object)
		{
			return nullptr;
		}
	};
}


#define DECLARE_MULTICAST_DELEGATE(DelegateName, ...)				\
	using DelegateName = ::libty::MulticastDelegate<void(__VA_ARGS__)>;
