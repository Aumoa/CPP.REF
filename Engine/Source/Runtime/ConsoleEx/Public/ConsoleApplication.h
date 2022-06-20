// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "Threading/Tasks/Task.h"
#include <functional>
#include "ConsoleApplication.generated.h"

class CommandLineBuilder;

SCLASS()
class CONSOLEEX_API ConsoleApplication : virtual public Object
{
	GENERATED_BODY()

private:
	using ApplicationFactory_t = ConsoleApplication*(*)();
	using InvokeMain_t = int32(*)(ConsoleApplication*, const CommandLineBuilder&);

public:
	ConsoleApplication();
	virtual ~ConsoleApplication() noexcept override;

	template<class TApplication>
	static inline int32 GuardedMain(int32 argc, char** argv)
	{
		static_assert(std::same_as<TApplication, void>, "TApplication does not have any supported Run method.");
	}

	template<class TApplication>
	static inline int32 GuardedMain(int32 argc, char** argv) requires
		requires
		{
			{ std::declval<TApplication>().Run() } -> std::convertible_to<int32>;
		}
	{
		static auto _InstanceFactory = +[]() -> ConsoleApplication* { return new TApplication(); };
		static auto _InvokeMain = +[](ConsoleApplication* app, const CommandLineBuilder&)
		{
			return static_cast<TApplication*>(app)->Run();
		};

		return GuardedMain(argc, argv, _InstanceFactory, _InvokeMain);
	}

	template<class TApplication>
	static inline int32 GuardedMain(int32 argc, char** argv) requires
		requires
	{
		{ std::declval<TApplication>().Run() } -> std::convertible_to<Task<int32>>;
	}
	{
		static auto _InstanceFactory = +[]() -> ConsoleApplication* { return new TApplication(); };
		static auto _InvokeMain = +[](ConsoleApplication* app, const CommandLineBuilder&)
		{
			return static_cast<TApplication*>(app)->Run().GetResult();
		};

		return GuardedMain(argc, argv, _InstanceFactory, _InvokeMain);
	}

private:
	static int32 GuardedMain(int32 argc, char** argv, ApplicationFactory_t factory, InvokeMain_t invokeMain);
	static int32 InvokeMain(int32 argc, char** argv, ApplicationFactory_t factory, InvokeMain_t invokeMain);
};