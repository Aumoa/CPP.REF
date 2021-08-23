// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"

struct Scale2D
{
	constexpr Scale2D()
	{
	}

	constexpr Scale2D(float uniformScale) : Scale(uniformScale, uniformScale)
	{
	}

	constexpr Scale2D(float scaleX, float scaleY) : Scale(scaleX, scaleY)
	{
	}

	constexpr Scale2D(const Vector2& scale) : Scale(scale)
	{
	}

	std::wstring ToString(std::wstring_view formatArgs = L"") const
	{
		return std::format(L"Scale: {}", Scale.ToString(formatArgs));
	}

	constexpr Vector2 TransformPoint(const Vector2& point) const
	{
		return Scale * point;
	}

	constexpr Vector2 TransformVector(const Vector2& vector) const
	{
		return TransformPoint(vector);
	}

	constexpr Scale2D Concatenate(const Scale2D& rhs) const
	{
		return Scale2D(Scale * rhs.Scale);
	}

	constexpr Scale2D Inverse() const
	{
		return Scale2D(1.0f / Scale.X, 1.0f / Scale.Y);
	}

	constexpr static float Concatenate(float lhs, float rhs)
	{
		return lhs * rhs;
	}

	constexpr const Vector2& GetVector() const
	{
		return Scale;
	}

	constexpr bool operator ==(const Scale2D& rhs) const
	{
		return Scale == rhs.Scale;
	}

	constexpr bool operator !=(const Scale2D& rhs) const
	{
		return Scale != rhs.Scale;
	}

	static constexpr Scale2D Identity()
	{
		return Scale2D(1.0f);
	}

private:
	Vector2 Scale;
};