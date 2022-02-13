// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreConcepts.h"

template<class T>
concept TIsTransformBase = requires
{
	{ T::Inverse(std::declval<T>()) };
	{ std::declval<T>().Inverse() };
	{ T::Concatenate(std::declval<T>(), std::declval<T>()) };
	{ std::declval<T>().Concatenate(std::declval<T>()) };
	{ T::Identity() };
};

template<class T, class U>
concept TIsTransform = TIsTransformBase<T> && requires
{
	{ T::TransformPoint(std::declval<T>(), std::declval<U>()) };
	{ std::declval<T>().TransformPoint(std::declval<U>()) };
	{ T::TransformVector(std::declval<T>(), std::declval<U>()) };
	{ std::declval<T>().TransformVector(std::declval<U>()) };
};