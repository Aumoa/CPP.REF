// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#define DECLARE_MULTICAST_DELEGATE(DelegateName, ...) using DelegateName = MulticastDelegate<void(__VA_ARGS__)>;