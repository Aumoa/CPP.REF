// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Launch.gen.h"

namespace Ayla
{
	class GenericApplication;
	class Engine;
	class DynamicLibrary;

	ACLASS()
	class LAUNCH_API Launch : public Object
	{
		GENERATED_BODY()

	private:
		GenericApplication* m_GenericApp;

	protected:
		Launch(GenericApplication* genericApp);

	public:
		virtual ~Launch() noexcept override;

		virtual int32 StartApplication();
		GenericApplication* GetApplication();

		static int32 GuardedMain(std::vector<String> args, DynamicLibrary& api);
	};
}