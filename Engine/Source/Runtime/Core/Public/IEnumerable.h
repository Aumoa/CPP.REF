// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Core/Object.h"

template<template<class> class T, class TItem>
interface IConstEnumerable
{
	virtual typename T<const TItem> cbegin() const = 0;
	virtual typename T<const TItem> cend() const = 0;
};

template<template<class> class T, class TItem>
interface IEnumerable : virtual public Object, virtual public IConstEnumerable<T, TItem>
{
	virtual typename T<TItem> begin() = 0;
	virtual typename T<TItem> end() = 0;
};