// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
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

	constexpr SlateRenderTransform(const Vector2& InTranslation)
		: M(Matrix2x2::Identity())
		, Translation(InTranslation)
	{
	}

	constexpr SlateRenderTransform(float UniformScale, const Vector2& InTranslation = Vector2::GetZero())
		: SlateRenderTransform(Scale2D(UniformScale), InTranslation)
	{
	}

	constexpr SlateRenderTransform(const Scale2D& Scale, const Vector2& InTranslation = Vector2::GetZero())
		: M(Matrix2x2::Scale(Scale.GetVector()))
		, Translation(InTranslation)
	{
	}

	constexpr SlateRenderTransform(const Shear2D& Shear, const Vector2& InTranslation = Vector2::GetZero())
		: M(Matrix2x2::Shear(Shear.GetVector()))
		, Translation(InTranslation)
	{
	}

	constexpr SlateRenderTransform(const Complex& Rotation, const Vector2& InTranslation = Vector2::GetZero())
		: M(Rotation.ToMatrix())
		, Translation(InTranslation)
	{
	}

	constexpr SlateRenderTransform(const Matrix2x2& Transform, const Vector2& InTranslation = Vector2::GetZero())
		: M(Transform)
		, Translation(InTranslation)
	{
	}

	constexpr SlateRenderTransform(const Matrix3x2& Transform)
		: M{ Transform.Get(0, 0), Transform.Get(0, 1), Transform.Get(1, 0), Transform.Get(1, 1) }
		, Translation(Transform.Get(2, 0), Transform.Get(2, 1))
	{
	}

	constexpr Vector2 TransformPoint(const Vector2& Point) const
	{
		return TransformCalculus2D::TransformPoint(Translation, TransformCalculus2D::TransformPoint(M, Point));
	}

	constexpr Vector2 TransformVector(const Vector2& Vector) const
	{
		return TransformCalculus2D::TransformVector(M, Vector);
	}

	constexpr SlateRenderTransform Concatenate(const SlateRenderTransform& Rhs) const
	{
		return SlateRenderTransform(
			Matrix2x2::Multiply(M, Rhs.M),
			TransformCalculus2D::Concatenate(TransformCalculus2D::TransformPoint(Rhs.M, Translation), Rhs.Translation)
		);
	}

	constexpr SlateRenderTransform Inverse() const
	{
		Matrix2x2 InvM = TransformCalculus2D::Inverse(M);
		Vector2 InvTrans = InvM.TransformPoint(-Translation);
		return SlateRenderTransform(InvM, InvTrans);
	}

	constexpr bool operator ==(const SlateRenderTransform& Rhs) const
	{
		return M == Rhs.M && Translation == Rhs.Translation;
	}

	constexpr bool operator !=(const SlateRenderTransform& Rhs) const
	{
		return M != Rhs.M || Translation != Rhs.Translation;
	}

	std::wstring ToString(std::wstring_view InFormatArgs = L"") const
	{
		return std::format(L"M: {}, Translation: {}", M.ToString(InFormatArgs), Translation.ToString(InFormatArgs));
	}

	constexpr bool IsIdentity() const
	{
		return M.IsIdentity() && Translation == Vector2::GetZero();
	}

	constexpr const Vector2& GetTranslation() const { return Translation; }
	void SetTranslation(const Vector2& InTranslation) { Translation = InTranslation; }
	constexpr const Matrix2x2& GetMatrix() const { return M; }

	static constexpr SlateRenderTransform Identity()
	{
		return SlateRenderTransform(Matrix2x2::Identity(), Vector2::GetZero());
	}

private:
	Matrix2x2 M = Matrix2x2::Identity();
	Vector2 Translation;
};