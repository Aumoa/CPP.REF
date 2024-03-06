// Copyright 2020-2022 Aumoa.lib. All right reserved.

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

	inline void Execute()
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
		body();
	}
};