// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class GenericApplication;

	class LAUNCH_API Launch : public virtual Object
	{
		using Super = This;
		using This = Launch;

	private:
		std::vector<String> CmdArgs;
		PPtr<GenericApplication> GenericApp;
		PPtr<Object> T1;
		PPtr<Object> T2;
		PPtr<Object> T3;
		PPtr<Object> T4;
		PPtr<Object> T5;

	protected:
		Launch(std::vector<String> CmdArgs);

		virtual void GatherProperties(PPtrCollection& collection) override;

	public:
		int32 GuardedMain();

		virtual void* GetApplicationPointer() = 0;
	};
}