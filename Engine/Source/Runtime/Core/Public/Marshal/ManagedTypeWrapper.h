// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

namespace Ayla
{
	class Type;

	struct ManagedTypeWrapper
	{
		const Type* NativeType;
		void* ScriptTypeGetter;
	};
}