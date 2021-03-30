// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<TIsBaseOf<AActor> T>
T* GActorComponent::GetOwner() const
{
	return Cast<T>(GetOwner());
}