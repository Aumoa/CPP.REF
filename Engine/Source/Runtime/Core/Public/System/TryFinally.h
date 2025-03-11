// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

template<class TBody, class TFinally>
class TryFinally
{
	TBody body;
	TFinally finally;

public:
	inline TryFinally(TBody&& body, TFinally&& finally)
		: body(std::forward<TBody>(body))
		, finally(std::forward<TFinally>(finally))
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
				self->finally();
			}
		};

		FinallyScope scope(this);
		return body();
	}
};