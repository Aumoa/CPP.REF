// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Launch.gen.h"

namespace Ayla
{
	class GenericApplication;

	ACLASS()
	class LAUNCH_API Launch : public Object
	{
		GENERATED_BODY()

	private:
		std::vector<String> CmdArgs;
		APROPERTY()
		PPtr<GenericApplication> GenericApp;

	protected:
		Launch(std::vector<String> CmdArgs);

	public:
		int32 GuardedMain();

		virtual void* GetApplicationPointer() = 0;
	};
}