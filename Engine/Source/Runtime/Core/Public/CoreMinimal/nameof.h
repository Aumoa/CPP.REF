// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#define nameof(x) lib::details::__nameof_helper<decltype(x)>(TEXT(#x))