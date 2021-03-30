// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Transform.h"

Transform::Transform()
{

}

Transform::Transform(const Vector3& translation, const Vector3& scale, const Quaternion& rotation)
{
	Translation = translation;
	Scale = scale;
	Rotation = rotation;
}

Transform::Transform(const Transform& copy)
{
	Translation = copy.Translation;
	Scale = copy.Scale;
	Rotation = copy.Rotation;
}

Transform::~Transform()
{

}

bool Transform::Equals(const Transform& rh) const
{
	return *this == rh;
}

bool Transform::NearlyEquals(const Transform& rh, float epsilon) const
{
	return Translation.NearlyEquals(rh.Translation, epsilon)
		&& Scale.NearlyEquals(rh.Scale, epsilon)
		&& Rotation.NearlyEquals(rh.Rotation, epsilon);
}

size_t Transform::GetHashCode() const
{
	return Translation.GetHashCode() ^ Scale.GetHashCode() ^ Rotation.GetHashCode();
}

TRefPtr<String> Transform::ToString() const
{
	return String::Format("{{T: {0}, S: {1}, R: {2}}}", Translation, Scale, Rotation);
}

Vector3 Transform::TransformVector(const Vector3& v) const
{
	return TransformVector(Vector4(v.X, v.Y, v.Z, 1)).Cast<Vector3>();
}

Vector4 Transform::TransformVector(const Vector4& v) const
{
	const Vector4 scaled = v * Vector4(Scale, 1.0);
	const Vector4 rotated = Rotation.RotateVector(scaled);
	return rotated + Vector4(Translation * v.W, v.W);
}

Transform Transform::GetRelativeTransform(const Transform& rh) const
{
	Vector3 recipScale = 1.0f / rh.Scale;
	Quaternion invRotation = rh.Rotation.Inverse;

	Transform r;
	r.Scale = Scale * recipScale;
	r.Rotation = invRotation * Rotation;
	r.Translation = invRotation.RotateVector(Translation - rh.Translation) * recipScale;

	return r;
}

Transform Transform::Inverse_get() const
{
	Transform inverse;
	inverse.Translation = -Translation;
	inverse.Scale = 1.0f / Scale;
	inverse.Rotation = Rotation.Inverse;
	return inverse;
}

Matrix4x4 Transform::Matrix_get() const
{
	return Matrix4x4::AffineTransformation(Translation, Scale, Rotation);
}

bool Transform::operator ==(const Transform& rh) const
{
	return Translation == rh.Translation && Scale == rh.Scale && Rotation == rh.Rotation;
}

bool Transform::operator !=(const Transform& rh) const
{
	return Translation != rh.Translation || Scale != rh.Scale || Rotation != rh.Rotation;
}

Transform Transform::FromMatrix(const Matrix4x4& value)
{
	Transform t;
	value.BreakTransform(t.Translation, t.Scale, t.Rotation);
	return t;
}

Transform Transform::Multiply(const Transform& lh, const Transform& rh)
{
	Matrix4x4 m = Matrix4x4::Multiply(lh.Matrix, rh.Matrix);
	Transform trp;
	m.BreakTransform(trp.Translation, trp.Scale, trp.Rotation);
	return trp;
}

Transform Transform::Identity = Transform(Vector3::Zero, Vector3::One, Quaternion::Identity);