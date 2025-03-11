// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

struct NGenericPlatformInputMouseMoveEvent
{
	Vector2N Location;
};

enum EGenericPlatformInputMouseButtonType
{
	Left,
	Right,
	Middle
};

struct NGenericPlatformInputMouseButtonEvent
{
	EGenericPlatformInputMouseButtonType ButtonType;
	bool bUp;
	Vector2N Location;
};

struct NGenericPlatformInputEvent
{
	static constexpr size_t BufSize = Math::Max(sizeof(NGenericPlatformInputMouseMoveEvent), sizeof(NGenericPlatformInputMouseButtonEvent));
	byte Idx = 0;
	byte Buf[BufSize] = {};

	static constexpr size_t IDX_MouseMove = 0;
	NGenericPlatformInputMouseMoveEvent& MouseMove() { return reinterpret_cast<NGenericPlatformInputMouseMoveEvent&>(*Buf); }
	const NGenericPlatformInputMouseMoveEvent& MouseMove() const { return reinterpret_cast<const NGenericPlatformInputMouseMoveEvent&>(*Buf); }

	static constexpr size_t IDX_MouseButton = 1;
	NGenericPlatformInputMouseButtonEvent& MouseButton() { return reinterpret_cast<NGenericPlatformInputMouseButtonEvent&>(*Buf); }
	const NGenericPlatformInputMouseButtonEvent& MouseButton() const { return reinterpret_cast<const NGenericPlatformInputMouseButtonEvent&>(*Buf); }
};