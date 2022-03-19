// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY(GAME_API, LogConsoleVar);

namespace Details
{
	class GAME_API AutoConsoleVariableBase
	{
	private:
		std::wstring Name;

	public:
		AutoConsoleVariableBase(std::wstring_view Key);

		std::wstring_view GetName() const;

	public:
		static bool TryProcessConsoleVar(std::wstring_view Key, std::wstring_view Argument);

	protected:
		virtual void ProcessConsoleVar(std::wstring_view Argument) = 0;
	};
}

namespace ConsoleVars
{
	namespace Details
	{
		template<class U, class T>
		inline U CheckOutOfRange(T Value)
		{
			static constexpr const T Minimum = (T)std::numeric_limits<U>::lowest();
			static constexpr const T Maximum = (T)std::numeric_limits<U>::max();
			if (Value < Minimum || Value > Maximum)
			{
				throw std::out_of_range("Value is out of range.");
			}
			return (U)Value;
		}

		inline void Parse(std::wstring_view Source, wchar_t* Out)
		{
			if (Source.length() > 1)
			{
				throw std::out_of_range("String length is out of range.");
			}

			*Out = Source[0];
		}

		inline void Parse(std::wstring_view Source, char* Out)
		{
			if (Source.length() > 1)
			{
				throw std::out_of_range("String length is out of range.");
			}

			*Out = (char)Source[0];
		}

		inline void Parse(std::wstring_view Source, uint16* Out)
		{
			int32 Value = std::stoi(std::wstring(Source), nullptr, 10);
			*Out = CheckOutOfRange<uint16>(Value);
		}

		inline void Parse(std::wstring_view Source, int16* Out)
		{
			int32 Value = std::stoi(std::wstring(Source), nullptr, 10);
			*Out = CheckOutOfRange<int16>(Value);
		}

		inline void Parse(std::wstring_view Source, uint32* Out)
		{
			*Out = std::stoul(std::wstring(Source), nullptr, 10);
		}

		inline void Parse(std::wstring_view Source, int32* Out)
		{
			*Out = std::stoi(std::wstring(Source), nullptr, 10);
		}

		inline void Parse(std::wstring_view Source, uint64* Out)
		{
			*Out = std::stoull(std::wstring(Source), nullptr, 10);
		}

		inline void Parse(std::wstring_view Source, int64* Out)
		{
			*Out = std::stoll(std::wstring(Source), nullptr, 10);
		}

		inline void Parse(std::wstring_view Source, float* Out)
		{
			*Out = std::stof(std::wstring(Source), nullptr);
		}

		inline void Parse(std::wstring_view Source, double* Out)
		{
			*Out = std::stod(std::wstring(Source), nullptr);
		}
	}

	template<class T>
	inline bool TryParse(std::wstring_view Source, T* Out) requires requires
	{
		{ Details::Parse(Source, Out) };
	}
	{
		try
		{
			Details::Parse(Source, Out);
			return true;
		}
		catch (const std::invalid_argument& E)
		{
			SE_LOG(LogConsoleVar, Warning, L"Invalid argument detected while parsing console variable: {}", String::AsUnicode(E.what()));
			return false;
		}
		catch (const std::out_of_range& E)
		{
			SE_LOG(LogConsoleVar, Warning, L"Out of range detected while parsing console variable: {}", String::AsUnicode(E.what()));
			return false;
		}
	}

	template<class T>
	inline bool TryParse(std::wstring_view Source, T* Out) requires requires
	{
		{ T::TryParse(Source, Out) } -> std::same_as<bool>;
	}
	{
		return T::TryParse(Source, Out);
	}
}

template<class T>
class AutoConsoleVariable : public Details::AutoConsoleVariableBase
{
	T Value;

private:
	using This = AutoConsoleVariable;

public:
	template<class U>
	AutoConsoleVariable(std::wstring_view Key, U&& InitialValue)
		: AutoConsoleVariableBase(Key)
		, Value(std::forward<U>(InitialValue))
	{
	}

	void ProcessConsoleVar(std::wstring_view Argument)
	{
		if (Argument.empty())
		{
			SE_LOG(LogConsoleVar, Verbose, L"{}: {}", GetName(), Value);
		}
		else
		{
			if (ConsoleVars::TryParse(Argument, &Value))
			{
				VariableCommitted.Broadcast(*this);
			}
			SE_LOG(LogConsoleVar, Verbose, L"{} = {}", GetName(), Value);
		}
	}

	T GetValue() const
	{
		return Value;
	}

	DECLARE_MULTICAST_EVENT(VariableCommittedDelegate, AutoConsoleVariable<T>&);
	VariableCommittedDelegate VariableCommitted;
};