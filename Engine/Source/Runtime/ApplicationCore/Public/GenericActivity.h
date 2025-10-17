// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericActivity.gen.h"

namespace Ayla
{
	class GenericWindow;

	ACLASS()
	class APPLICATIONCORE_API GenericActivity : public Object
	{
		GENERATED_BODY()

	private:
		GenericActivity(const GenericActivity&) = delete;
		GenericActivity& operator=(const GenericActivity&) = delete;

	public:
		GenericActivity();
		virtual ~GenericActivity() noexcept;

		AFUNCTION()
		virtual void BeforeInitialize() APURE;
		AFUNCTION()
		virtual void AfterInitialize() APURE;
		AFUNCTION()
		virtual SharedPtr<GenericWindow> GetMainWindow() const APURE;
	};
}