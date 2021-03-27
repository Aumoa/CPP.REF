// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Pawn.h"

template<TIsController T>
T* APawn::GetController() const
{
	return Cast<T>(GetController());
}