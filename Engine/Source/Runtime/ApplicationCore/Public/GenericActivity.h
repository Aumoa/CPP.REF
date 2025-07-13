// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class GenericWindow;

	class APPLICATIONCORE_API GenericActivity
	{
		GenericActivity(const GenericActivity&) = delete;
		GenericActivity& operator=(const GenericActivity&) = delete;

	public:
		GenericActivity();
		virtual ~GenericActivity() noexcept;

		virtual void BeforeInitialize() = 0;
		virtual void AfterInitialize() = 0;
		virtual std::shared_ptr<GenericWindow> GetMainWindow() const = 0;
	};
}