// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct Scale2D
{
	constexpr Scale2D()
	{
	}

	constexpr Scale2D(float UniformScale) : Scale(UniformScale, UniformScale)
	{
	}

	constexpr Scale2D(float ScaleX, float ScaleY) : Scale(ScaleX, ScaleY)
	{
	}

	constexpr Scale2D(const Vector2& Scale) : Scale(Scale)
	{
	}

	std::wstring ToString(std::wstring_view InFormatArgs = L"") const
	{
		return std::format(L"Scale: {}", Scale.ToString(InFormatArgs));
	}

	constexpr Vector2 TransformPoint(const Vector2& Point) const
	{
		return Scale * Point;
	}

	constexpr Vector2 TransformVector(const Vector2& Vector) const
	{
		return TransformPoint(Vector);
	}

	constexpr Scale2D Concatenate(const Scale2D& Rhs) const
	{
		return Scale2D(Scale * Rhs.Scale);
	}

	constexpr Scale2D Inverse() const
	{
		return Scale2D(1.0f / Scale.X, 1.0f / Scale.Y);
	}

	constexpr static float Concatenate(float Lhs, float Rhs)
	{
		return Lhs * Rhs;
	}

	constexpr const Vector2& GetVector() const
	{
		return Scale;
	}

	constexpr bool operator ==(const Scale2D& Rhs) const
	{
		return Scale == Rhs.Scale;
	}

	constexpr bool operator !=(const Scale2D& Rhs) const
	{
		return Scale != Rhs.Scale;
	}

	static constexpr Scale2D Identity()
	{
		return Scale2D(1.0f);
	}

private:
	Vector2 Scale;
};