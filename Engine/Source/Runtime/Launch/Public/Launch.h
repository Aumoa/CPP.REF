// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class GenericApplication;
	class Engine;
	class PlatformRenderFeature;

	ACLASS()
	class LAUNCH_API Launch : public Object
	{
		GENERATED_BODY()

	private:
		std::shared_ptr<GenericApplication> m_GenericApp;
		std::shared_ptr<Engine> m_Engine;

	protected:
		Launch();

	public:
		virtual ~Launch() noexcept override;

		virtual int32 StartApplication();
		std::shared_ptr<GenericApplication> GetApplication();

		virtual void* GetApplicationPointer() = 0;
		virtual std::shared_ptr<PlatformRenderFeature> CreatePlatformRenderFeature() = 0;
	};
}