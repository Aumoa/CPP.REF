// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Console
{
	class InternalConsole;

	class ConsolePin
	{
		friend class InternalConsole;

		struct Impl
		{
			int32 _y;
			int32 _height;
		};

		std::shared_ptr<Impl> _impl;

	public:
		ConsolePin() noexcept = default;
		ConsolePin(const ConsolePin&) noexcept = default;
		ConsolePin(ConsolePin&&) noexcept = default;
		~ConsolePin() noexcept = default;

		inline bool IsValid() const
		{
			return (bool)_impl;
		}

		static inline ConsolePin Null() noexcept { return {}; }
		CONSOLE_API static ConsolePin Reserve(int32 height = 1);
	};
}