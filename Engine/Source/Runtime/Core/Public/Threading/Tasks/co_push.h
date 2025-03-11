// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

struct co_push_adaptor
{
};

template<class T>
struct co_push_t
{
};

#define co_push co_push_adaptor() << 