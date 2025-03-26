// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <type_traits>

namespace Ayla
{
	template<class TBody, class TFinally>
	class TryFinally
	{
		TBody body;
		TFinally finally_;

	public:
		inline TryFinally(TBody&& body, TFinally&& finally_)
			: body(std::forward<TBody>(body))
			, finally_(std::forward<TFinally>(finally_))
		{
		}

		inline ~TryFinally() noexcept
		{
		}

		inline std::invoke_result_t<TBody> Execute()
		{
			struct FinallyScope
			{
				const TryFinally* self;

				FinallyScope(const TryFinally* self) : self(self)
				{
				}

				inline ~FinallyScope() noexcept
				{
					self->finally_();
				}
			};

			FinallyScope scope(this);
			return body();
		}
	};
}