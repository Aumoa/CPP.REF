// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

DECLARE_LOG_CATEGORY(GAME_API, LogConsoleVar);

namespace libty::inline Game
{
	namespace Details
	{
		class GAME_API AutoConsoleVariableBase
		{
		private:
			String Name;

		public:
			AutoConsoleVariableBase(StringView Key);

			StringView GetName() const;

		public:
			static bool TryProcessConsoleVar(StringView Key, StringView Argument);

		protected:
			virtual void ProcessConsoleVar(StringView Argument) = 0;
		};

		template<class U, class T>
		inline U CheckOutOfRange(T Value)
		{
			static constexpr const T Minimum = (T)std::numeric_limits<U>::lowest();
			static constexpr const T Maximum = (T)std::numeric_limits<U>::max();
			if (Value < Minimum || Value > Maximum)
			{
				throw ArgumentException(TEXT("Value is out of range."));
			}
			return (U)Value;
		}

		inline void Parse(StringView Source, wchar_t* Out)
		{
			if (Source.length() > 1)
			{
				throw ArgumentException(TEXT("String length is out of range."));
			}

			*Out = Source[0];
		}

		inline void Parse(StringView Source, char* Out)
		{
			if (Source.length() > 1)
			{
				throw ArgumentException(TEXT("String length is out of range."));
			}

			*Out = (char)Source[0];
		}

		inline void Parse(StringView Source, uint16* Out)
		{
			int32 Value = std::stoi(String(Source), nullptr, 10);
			*Out = CheckOutOfRange<uint16>(Value);
		}

		inline void Parse(StringView Source, int16* Out)
		{
			int32 Value = std::stoi(String(Source), nullptr, 10);
			*Out = CheckOutOfRange<int16>(Value);
		}

		inline void Parse(StringView Source, uint32* Out)
		{
			*Out = std::stoul(String(Source), nullptr, 10);
		}

		inline void Parse(StringView Source, int32* Out)
		{
			*Out = std::stoi(String(Source), nullptr, 10);
		}

		inline void Parse(StringView Source, uint64* Out)
		{
			*Out = std::stoull(String(Source), nullptr, 10);
		}

		inline void Parse(StringView Source, int64* Out)
		{
			*Out = std::stoll(String(Source), nullptr, 10);
		}

		inline void Parse(StringView Source, float* Out)
		{
			*Out = std::stof(String(Source), nullptr);
		}

		inline void Parse(StringView Source, double* Out)
		{
			*Out = std::stod(String(Source), nullptr);
		}

		template<class T>
		inline bool TryParse(StringView Source, T* Out) requires requires
		{
			{ Parse(Source, Out) };
		}
		{
			try
			{
				Parse(Source, Out);
				return true;
			}
			catch (const std::invalid_argument& E)
			{
				SE_LOG(LogConsoleVar, Warning, TEXT("Invalid argument detected while parsing console variable: {}"), String::AsUnicode(E.what()));
				return false;
			}
			catch (const std::out_of_range& E)
			{
				SE_LOG(LogConsoleVar, Warning, TEXT("Out of range detected while parsing console variable: {}"), String::AsUnicode(E.what()));
				return false;
			}
		}

		template<class T>
		inline bool TryParse(StringView Source, T* Out) requires requires
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
		AutoConsoleVariable(StringView Key, U&& InitialValue)
			: AutoConsoleVariableBase(Key)
			, Value(std::forward<U>(InitialValue))
		{
		}

		void ProcessConsoleVar(StringView Argument)
		{
			if (Argument.empty())
			{
				SE_LOG(LogConsoleVar, Verbose, TEXT("{}: {}"), GetName(), Value);
			}
			else
			{
				if (Details::TryParse(Argument, &Value))
				{
					VariableCommitted.Broadcast(*this);
				}
				SE_LOG(LogConsoleVar, Verbose, TEXT("{} = {}"), GetName(), Value);
			}
		}

		T GetValue() const
		{
			return Value;
		}

		DECLARE_MULTICAST_EVENT(VariableCommittedDelegate, AutoConsoleVariable<T>&);
		VariableCommittedDelegate VariableCommitted;
	};
}