// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "Scale2D.h"
#include "Shear2D.h"
#include "TransformCalculus2D.h"
#include <format>
#include <string>

struct SlateLayoutTransform;

struct SlateRenderTransform
{
	constexpr SlateRenderTransform()
	{
	}

	constexpr SlateRenderTransform(const Vector2& translation)
		: M(Matrix2x2::Identity())
		, Translation(translation)
	{
	}

	constexpr SlateRenderTransform(float uniformScale, const Vector2& translation = Vector2::GetZero())
		: SlateRenderTransform(Scale2D(uniformScale), translation)
	{
	}

	constexpr SlateRenderTransform(const Scale2D& scale, const Vector2& translation = Vector2::GetZero())
		: M(Matrix2x2::Scale(scale.GetVector()))
		, Translation(translation)
	{
	}

	constexpr SlateRenderTransform(const Shear2D& shear, const Vector2& translation = Vector2::GetZero())
		: M(Matrix2x2::Shear(shear.GetVector()))
		, Translation(translation)
	{
	}

	constexpr SlateRenderTransform(const Complex& rotation, const Vector2& translation = Vector2::GetZero())
		: M(rotation.ToMatrix())
		, Translation(translation)
	{
	}

	constexpr SlateRenderTransform(const Matrix2x2& transform, const Vector2& translation = Vector2::GetZero())
		: M(transform)
		, Translation(translation)
	{
	}

	constexpr SlateRenderTransform(const Matrix3x2& transform)
		: M{ transform.Get(0, 0), transform.Get(0, 1), transform.Get(1, 0), transform.Get(1, 1) }
		, Translation(transform.Get(2, 0), transform.Get(2, 1))
	{
	}

	constexpr Vector2 TransformPoint(const Vector2& point) const
	{
		return TransformCalculus2D::TransformPoint(Translation, TransformCalculus2D::TransformPoint(M, point));
	}

	constexpr Vector2 TransformVector(const Vector2& vector) const
	{
		return TransformCalculus2D::TransformVector(M, vector);
	}

	constexpr SlateRenderTransform Concatenate(const SlateRenderTransform& rhs) const
	{
		return SlateRenderTransform(
			Matrix2x2::Multiply(M, rhs.M),
			TransformCalculus2D::Concatenate(TransformCalculus2D::TransformPoint(rhs.M, Translation), rhs.Translation)
		);
	}

	constexpr SlateRenderTransform Inverse() const
	{
		Matrix2x2 invM = TransformCalculus2D::Inverse(M);
		Vector2 invTrans = invM.TransformPoint(-Translation);
		return SlateRenderTransform(invM, invTrans);
	}

	constexpr bool operator ==(const SlateRenderTransform& rhs) const
	{
		return M == rhs.M && Translation == rhs.Translation;
	}

	constexpr bool operator !=(const SlateRenderTransform& rhs) const
	{
		return M != rhs.M || Translation != rhs.Translation;
	}

	std::wstring ToString(std::wstring_view formatArgs = L"") const
	{
		return std::format(L"M: {}, Translation: {}", M.ToString(formatArgs), Translation.ToString(formatArgs));
	}

	constexpr bool IsIdentity() const
	{
		return M.IsIdentity() && Translation == Vector2::GetZero();
	}

	constexpr const Vector2& GetTranslation() const { return Translation; }
	void SetTranslation(const Vector2& translation) { Translation = translation; }
	constexpr const Matrix2x2& GetMatrix() const { return M; }

	static constexpr SlateRenderTransform Identity()
	{
		return SlateRenderTransform(Matrix2x2::Identity(), Vector2::GetZero());
	}

private:
	Matrix2x2 M = Matrix2x2::Identity();
	Vector2 Translation;
};