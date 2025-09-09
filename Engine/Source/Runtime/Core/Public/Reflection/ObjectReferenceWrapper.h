// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"

namespace Ayla
{
	class Object;
	enum Object::CreationFlags;

	struct ObjectReferenceWrapper
	{
		ssize_t InstanceId;
		ssize_t Handle;
		Object::CreationFlags Flags;
	};
}