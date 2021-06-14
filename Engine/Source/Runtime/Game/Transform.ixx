// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:Transform;

import std.core;
import SC.Runtime.Core;

using namespace std;

/// <summary>
/// Represents affine transformation that contains translation, scale and rotation.
/// </summary>
export struct Transform
{
	Vector3 Translation;
	Vector3 Scale = Vector3(1.0f, 1.0f, 1.0f);
	Quaternion Rotation = Quaternion::GetIdentity();

	/// <summary>
	/// Initialize new <see cref="Transform"/> instance.
	/// </summary>
	constexpr Transform()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Transform"/> instance.
	/// </summary>
	constexpr Transform(const Vector3& t, const Vector3& s, const Quaternion& q)
		: Translation(t)
		, Scale(s)
		, Rotation(q)
	{
	}

	/// <summary>
	/// Compare nearly equals to target transform with epsilon value.
	/// </summary>
	/// <param name="rhs"> The target transform. </param>
	/// <param name="epsilon"> The epsilon value. If different of two components is lower than this values, is nearly equals. </param>
	/// <returns> Indicate two transforms is nearly equals. </returns>
	constexpr bool NearlyEquals(const Transform& rhs, float epsilon) const
	{
		return Translation.NearlyEquals(rhs.Translation, epsilon)
			&& Scale.NearlyEquals(rhs.Scale, epsilon)
			&& Rotation.NearlyEquals(rhs.Rotation, epsilon);
	}

	/// <summary>
	/// Get simple string represents this transform value.
	/// </summary>
	/// <param name="formatArgs"> The formatting args that use to std::format. </param>
	/// <returns> The simple string value. </returns>
	wstring ToString(wstring_view formatArgs = L"") const
	{
		return format(L"{{T: {}, S: {}, R: {}}}", Translation.ToString(formatArgs), Scale.ToString(formatArgs), Rotation.ToString(formatArgs));
	}

	/// <summary>
	/// Transform point.
	/// </summary>
	constexpr Vector3 TransformPoint(const Vector3& v) const
	{
		return TransformVector(Vector4(v, 1.0f)).Swiz<0, 1, 2>();
	}

	/// <summary>
	/// Transform normal.
	/// </summary>
	constexpr Vector3 TransformNormal(const Vector3& v) const
	{
		return TransformVector(Vector4(v, 0.0f)).Swiz<0, 1, 2>();
	}

	/// <summary>
	/// Transform homogeneous coordinates.
	/// </summary>
	constexpr Vector4 TransformVector(const Vector4& v) const
	{
		const Vector4 scaled = v * Vector4(Scale, 1.0);
		const Vector4 rotated = Rotation.RotateVector(scaled);
		return rotated + Vector4(Translation * v.W(), v.W());
	}

	/// <summary>
	/// Get relative transform.
	/// </summary>
	static constexpr Transform GetRelativeTransform(const Transform& lhs, const Transform& rhs)
	{
		Vector3 recipScale = 1.0f / rhs.Scale;
		Quaternion invRotation = rhs.Rotation.GetInverse();

		Transform r =
		{
			invRotation.RotateVector(lhs.Translation - rhs.Translation)* recipScale,
			lhs.Scale * recipScale,
			invRotation * lhs.Rotation
		};

		return r;
	}

	/// <summary>
	/// Get inversed transform.
	/// </summary>
	constexpr Transform GetInverse() const
	{
		return
		{
			-Translation,
			1.0f / Scale,
			Rotation.GetInverse()
		};
	}

	/// <summary>
	/// Get affine transformation matrix.
	/// </summary>
	Matrix4x4 GetMatrix() const
	{
		return Matrix4x4::AffineTransformation(Translation, Scale, Rotation);
	}

	constexpr bool operator ==(const Transform& rhs) const
	{
		return Translation == rhs.Translation && Scale == rhs.Scale && Rotation == rhs.Rotation;
	}

	constexpr bool operator !=(const Transform& rhs) const
	{
		return Translation != rhs.Translation || Scale != rhs.Scale || Rotation != rhs.Rotation;
	}

	/// <summary>
	/// Multiply two transforms.
	/// </summary>
	static Transform Multiply(const Transform& lhs, const Transform& rhs)
	{
		Matrix4x4 M = Matrix4x4::Multiply(lhs.GetMatrix(), rhs.GetMatrix());
		Transform T;
		M.Decompose(T.Translation, T.Scale, T.Rotation);
		return T;
	}

	/// <summary>
	/// Get identity transform.
	/// </summary>
	static inline constexpr Transform GetIdentity();
};

inline constexpr Transform Transform::GetIdentity()
{
	return Transform(Vector3(), Vector3(), Quaternion::GetIdentity());
}