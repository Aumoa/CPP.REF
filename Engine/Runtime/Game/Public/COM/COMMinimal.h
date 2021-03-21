// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "COM/TComPtr.h"
#include "COM/COMException.h"

#define HR(X) if (auto hr = (X); FAILED(hr)) { throw COMException(hr); }