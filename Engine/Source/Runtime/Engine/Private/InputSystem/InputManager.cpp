// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "InputSystem/InputManager.h"

namespace Ayla
{
	InputManager::InputManager()
		: m_Keys{}
	{
	}

	InputManager::~InputManager() noexcept
	{
	}

	void InputManager::OnKeyDown(KeyCode keyCode)
	{
		m_Keys[(size_t)keyCode] = true;
	}

	void InputManager::OnKeyUp(KeyCode keyCode)
	{
		m_Keys[(size_t)keyCode] = false;
	}
}