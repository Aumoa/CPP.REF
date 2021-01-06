// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

template<TIsBaseOf<AActor> T>
T* ActorComponent::GetOwner() const
{
	return Cast<T>(GetOwner());
}