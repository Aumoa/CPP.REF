// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <functional>

template<class... TArgs>
using Action = std::function<void(TArgs...)>;