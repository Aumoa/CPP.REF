// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Launch.gen.h"

namespace Ayla
{
	class Engine;
	class DynamicLibrary;

	ACLASS()
	class LAUNCH_API Launch : public Object
	{
		GENERATED_BODY()

	private:
		APROPERTY()
		PPtr<Engine> m_Engine;

	protected:
		Launch();

	public:
		virtual ~Launch() noexcept override;

		virtual int32 StartApplication();

		static int32 GuardedMain(std::vector<String> args, DynamicLibrary& api);
	};
}