// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/String.h"
#include "Concepts/IArray.h"
#include "Exceptions/ArgumentException.h"
#include "Linq/IsValidIndex.h"
#include <string_view>
#include <span>
#include <map>

namespace libty::inline Core
{
	class CommandLine
	{
	private:
		std::map<String, std::vector<String>> _values;

	public:
		template<IArray<String> TArray>
		inline CommandLine(const TArray& args)
		{
			this->_Impl_parse_all(args);
		}

		inline bool ContainsKey(String key) const noexcept
		{
			return _values.contains(key);
		}

		inline bool TryGetValue(String key, size_t idx, String& oValue) const noexcept
		{
			auto it = _values.find(key);
			if (it == _values.end())
			{
				return false;
			}

			if (!Linq::IsValidIndex(it->second, idx))
			{
				return false;
			}

			oValue = it->second[idx];
			return true;
		}

		inline bool TryGetValues(String key, std::vector<String>& oValues) const noexcept
		{
			auto it = _values.find(key);
			if (it == _values.end())
			{
				return false;
			}

			oValues = it->second;
			return true;
		}

	private:
		template<IArray<String> TArray>
		inline void _Impl_parse_all(const TArray& args)
		{
			std::vector<String>* curr = nullptr;

			for (const String& str : args)
			{
				if (str.StartsWith(TEXT("--")))
				{
					String key = str.Substring(2);
					curr = &_values[key];
				}
				else
				{
					if (curr == nullptr)
					{
						throw ArgumentException(TEXT("The parameter name does not specified."));
					}

					curr->emplace_back(str);
					curr = nullptr;
				}
			}
		}
	};
}