// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <compare>
#include <utility>
#include "TRefPtr.h"
#include "Mathematics/TDegrees.h"
#include "Mathematics/TRadians.h"

namespace SC::Runtime::Core::Numerics
{
	struct Vector3;
	struct Matrix4x4;

#pragma pack(push, 8)
	struct CORE_API Quaternion
	{
		double X;
		double Y;
		double Z;
		double W;

		Quaternion();
		Quaternion(double x, double y, double z, double w);
		Quaternion(const Vector3& xyz, double w);
		Quaternion(double splat);
		Quaternion(const Quaternion& copy);

		bool Equals(const Quaternion& rh) const;
		bool NearlyEquals(const Quaternion& rh, double epsilon) const;
		size_t GetHashCode() const;
		TRefPtr<String> ToString() const;

		double GetComponentOrDefault(size_t index) const;
		template<TIsVectorType T>
		inline void Construct(const T& vector);
		template<TIsVectorType T>
		inline T Cast() const;
		bool Contains(size_t index) const;

		vs_property_get(size_t, Count);
		size_t Count_get() const;

		std::pair<Vector3, double> ToAxisAngle() const;
		Vector3 RotateVector(const Vector3& v) const;

		vs_property_get(double, LengthSq);
		double LengthSq_get() const;
		vs_property_get(double, Length);
		double Length_get() const;
		vs_property_get(Quaternion, Normalized);
		Quaternion Normalized_get() const;
		vs_property(Quaternion, Direction);
		Quaternion Direction_get() const;
		void Direction_set(const Quaternion& value);
		vs_property(Vector3, VectorPart);
		Vector3 VectorPart_get() const;
		void VectorPart_set(const Vector3& value);
		vs_property_get(double, Angle);
		double Angle_get() const;
		vs_property_get(Vector3, Axis);
		Vector3 Axis_get() const;
		vs_property_get(Quaternion, Inverse);
		Quaternion Inverse_get() const;

		const double& operator [](size_t index) const;
		double& operator [](size_t index);

		Quaternion operator -() const;
		Quaternion operator +(const Quaternion& right) const;
		Quaternion operator -(const Quaternion& right) const;
		Quaternion operator *(const Quaternion& right) const;
		Quaternion operator /(const Quaternion& right) const;
		double operator |(const Quaternion& right) const;

		bool operator ==(const Quaternion& right) const;
		bool operator !=(const Quaternion& right) const;
		bool operator < (const Quaternion& right) const;
		bool operator <=(const Quaternion& right) const;
		bool operator > (const Quaternion& right) const;
		bool operator >=(const Quaternion& right) const;
		std::weak_ordering operator <=>(const Quaternion& right) const;

		Quaternion& operator +=(const Quaternion& right);
		Quaternion& operator -=(const Quaternion& right);
		Quaternion& operator *=(const Quaternion& right);
		Quaternion& operator /=(const Quaternion& right);

		static double DotProduct(const Quaternion& left, const Quaternion& right);
		static Quaternion FromAxisAngle(const Vector3& axis, Mathematics::TDegrees<double> angle);
		static Quaternion Concatenate(const Quaternion& left, const Quaternion& right);
		static Quaternion FromMatrix(const Matrix4x4& rotationMatrix);
		static Quaternion FromEuler(Mathematics::TDegrees<double> yaw, Mathematics::TDegrees<double> pitch, Mathematics::TDegrees<double> roll);
		static Quaternion Lerp(const Quaternion& left, const Quaternion& right, double t);
		static Quaternion Slerp(const Quaternion& left, const Quaternion& right, double t);
		static Quaternion LookTo(const Vector3& forward, const Vector3& up);

		static Quaternion Identity;
	};
#pragma pack(pop)

	CORE_API Quaternion operator +(double left, const Quaternion& right);
	CORE_API Quaternion operator -(double left, const Quaternion& right);
	CORE_API Quaternion operator *(double left, const Quaternion& right);
	CORE_API Quaternion operator /(double left, const Quaternion& right);
}

#include "Quaternion.inl"