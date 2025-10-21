// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Components/Behavior.h"

namespace Ayla
{
	Behavior::Behavior() : Super()
	{
	}

	Behavior::~Behavior() noexcept
	{
	}

	void Behavior::SetEnabled(bool value)
	{
		m_Enabled = value;
	}
	
	bool Behavior::IsEnabled() const
	{
		return m_Enabled;
	}

	void Behavior::OnEnable_Implementation()
	{
	}

	void Behavior::OnDisable_Implementation()
	{
	}
}