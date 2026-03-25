// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericWindowExtension.h"
#include "IGenericWindowKeyEventHandler.h"
#include "InputManager.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API InputManager : public GenericWindowExtension, public IGenericWindowKeyEventHandler
	{
		GENERATED_BODY()

	private:
		static constexpr size_t kMaxKeyCode = 1024;
		bool m_Keys[kMaxKeyCode];

	public:
		InputManager();
		virtual ~InputManager() noexcept;

		virtual void OnKeyDown(KeyCode keyCode) override;
		virtual void OnKeyUp(KeyCode keyCode) override;

		bool GetKey(KeyCode keyCode) const noexcept { return m_Keys[(size_t)keyCode]; }
	};
}