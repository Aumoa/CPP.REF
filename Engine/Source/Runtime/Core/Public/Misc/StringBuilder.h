// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "String.h"
#include <vector>

namespace libty::inline Core
{
	class CORE_API StringBuilder
	{
		std::vector<String> _sources;
		size_t _length;

	public:
		StringBuilder();

		void Append(String str);
		void AppendLine(String str);
		void Insert(size_t idx, String str);
		void Clear() noexcept;

		template<class... TArgs>
		inline void AppendFormat(String format, TArgs&&... args)
		{
			Append(String::Format(format, std::forward<TArgs>(args)...));
		}

		template<class... TArgs>
		inline void AppendLineFormat(String format, TArgs&&... args)
		{
			AppendLine(String::Format(format, std::forward<TArgs>(args)...));
		}

		template<class... TArgs>
		inline void InsertFormat(String format, TArgs&&... args)
		{
			Insert(String::Format(format, std::forward<TArgs>(args)...));
		}

		[[nodiscard]] inline String ToString()
		{
			this->_Generate_string();
			if (_sources.empty())
			{
				return TEXT("");
			}
			else
			{
				return _sources.front();
			}
		}

		[[nodiscard]] size_t GetLength() const
		{
			return _length;
		}

	private:
		void _Generate_string();
	};
}