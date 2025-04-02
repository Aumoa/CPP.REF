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

	protected:
		Launch(std::vector<String> CmdArgs);

		virtual void GatherProperties(PropertyCollector& collection) override;

	public:
		int32 GuardedMain();

		virtual void* GetApplicationPointer() = 0;
	};
}