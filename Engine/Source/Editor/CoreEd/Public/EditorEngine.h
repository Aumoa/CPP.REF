// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "EditorEngine.gen.h"

namespace Ayla
{
	class EditorMainWindowContainer;

	ACLASS()
	class COREED_API EditorEngine : public Engine
	{
		GENERATED_BODY()

	private:
		APROPERTY()
		PPtr<EditorMainWindowContainer> m_EditorWindow;

	public:
		EditorEngine();
		virtual ~EditorEngine() noexcept override;

		virtual RPtr<InitializationContext> PreInitialize() override;
		virtual void Initialize(RPtr<InitializationContext> context, RPtr<PlatformRenderFeature> prf, RPtr<GenericApplication> app) override;

		virtual void Tick() override;

	protected:
		virtual void RenderWindows() override;
	};
}