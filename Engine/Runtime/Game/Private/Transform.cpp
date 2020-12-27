// Copyright 2020 Aumoa.lib. All right reserved.

#include "Transform.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Core::Numerics;
using namespace SC::Runtime::Game;

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

bool Transform::NearlyEquals(const Transform& rh, double epsilon) const
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
	return String::Format("{{Translation: {0}, Scale: {1}, Rotation: {2}}}", Translation, Scale, Rotation);
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

bool Transform::operator ==(const Transform& rh) const
{
	return Translation == rh.Translation && Scale == rh.Scale && Rotation == rh.Rotation;
}

bool Transform::operator !=(const Transform& rh) const
{
	return Translation != rh.Translation || Scale != rh.Scale || Rotation != rh.Rotation;
}

Transform Transform::Identity = Transform(Vector3::Zero, Vector3::One, Quaternion::Identity);