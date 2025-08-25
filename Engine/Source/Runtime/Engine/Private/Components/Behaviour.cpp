// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Components/Behaviour.h"

namespace Ayla
{
	Behaviour::Behaviour() : Super()
	{
	}

	Behaviour::~Behaviour() noexcept
	{
	}

	void Behaviour::SetEnabled(bool value)
	{
		m_Enabled = value;
	}
}