// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreConcepts.h"
#include <variant>
#include <memory>
#include <functional>

namespace libty::inline Core
{
	class Validator
	{
		Validator() = delete;
		Validator(const Validator&) = delete;

	private:
		using Variant_t = std::variant<std::weak_ptr<void>, std::function<bool()>>;
		Variant_t _variant;

	public:
		template<class T>
		Validator(const std::shared_ptr<T>& shrd_ptr) noexcept
			: _variant(std::weak_ptr<void>(shrd_ptr))
		{
		}

		Validator(std::function<bool()> pred) noexcept
			: _variant(std::move(pred))
		{
		}

		~Validator() noexcept
		{
		}

		bool IsValid() const
		{
			switch (_variant.index())
			{
			case 0:
				return !std::get<0>(_variant).expired();
			case 1:
				return std::get<1>(_variant)();
			}
			return false;
		}
	};
}