﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"

class TH_API Program : virtual public Object
{
public:
	using Super = Object;
	using This = Program;

public:
	static int32 Main();
};