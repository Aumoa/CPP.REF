// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	struct GenericPlatformInputMouseMoveEvent
	{
		Vector2N Location;
	};

	enum class GenericPlatformInputMouseButtonType
	{
		Left,
		Right,
		Middle
	};

	struct GenericPlatformInputMouseButtonEvent
	{
		GenericPlatformInputMouseButtonType ButtonType;
		bool bUp;
		Vector2N Location;
	};

	struct GenericPlatformInputEvent
	{
		static constexpr size_t BufSize = Math::Max(sizeof(GenericPlatformInputMouseMoveEvent), sizeof(GenericPlatformInputMouseButtonEvent));
		byte Idx = 0;
		byte Buf[BufSize] = {};

		static constexpr size_t IDX_MouseMove = 0;
		GenericPlatformInputMouseMoveEvent& MouseMove() { return reinterpret_cast<GenericPlatformInputMouseMoveEvent&>(*Buf); }
		const GenericPlatformInputMouseMoveEvent& MouseMove() const { return reinterpret_cast<const GenericPlatformInputMouseMoveEvent&>(*Buf); }

		static constexpr size_t IDX_MouseButton = 1;
		GenericPlatformInputMouseButtonEvent& MouseButton() { return reinterpret_cast<GenericPlatformInputMouseButtonEvent&>(*Buf); }
		const GenericPlatformInputMouseButtonEvent& MouseButton() const { return reinterpret_cast<const GenericPlatformInputMouseButtonEvent&>(*Buf); }
	};
}