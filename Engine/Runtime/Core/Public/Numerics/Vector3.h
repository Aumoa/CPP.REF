// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <compare>
#include "TRefPtr.h"

namespace SC::Runtime::Core::Numerics
{
	struct AxisAlignedCube;
	struct Vector2;

#pragma pack(push, 8)
	struct CORE_API Vector3
	{
		using This = Vector3;

		double X;
		double Y;
		double Z;

		Vector3();
		Vector3(const Vector2& v2, double z);
		Vector3(double x, double y, double z);
		Vector3(double splat);
		Vector3(const Vector3& copy);

		bool Equals(const Vector3& rh) const;
		bool NearlyEquals(const Vector3& rh, double epsilon) const;
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

		bool IsOverlap(const AxisAlignedCube& cube) const;

		vs_property_get(double, LengthSq);
		double LengthSq_get() const;
		vs_property_get(double, Length);
		double Length_get() const;
		vs_property_get(Vector3, Normalized);
		Vector3 Normalized_get() const;
		vs_property(Vector3, Direction);
		Vector3 Direction_get() const;
		void Direction_set(const Vector3& value);

		const double& operator [](size_t index) const;
		double& operator [](size_t index);

		Vector3 operator -() const;
		Vector3 operator +(const Vector3& right) const;
		Vector3 operator -(const Vector3& right) const;
		Vector3 operator *(const Vector3& right) const;
		Vector3 operator /(const Vector3& right) const;
		Vector3 operator ^(const Vector3& right) const;
		double operator |(const Vector3& right) const;

		bool operator ==(const Vector3& right) const;
		bool operator !=(const Vector3& right) const;
		bool operator < (const Vector3& right) const;
		bool operator <=(const Vector3& right) const;
		bool operator > (const Vector3& right) const;
		bool operator >=(const Vector3& right) const;
		std::weak_ordering operator <=>(const Vector3& right) const;

		Vector3& operator +=(const Vector3& right);
		Vector3& operator -=(const Vector3& right);
		Vector3& operator *=(const Vector3& right);
		Vector3& operator /=(const Vector3& right);
		Vector3& operator ^=(const Vector3& right);

		static double DistanceSq(const Vector3& left, const Vector3& right);
		static double Distance(const Vector3& left, const Vector3& right);
		static double DotProduct(const Vector3& left, const Vector3& right);
		static Vector3 CrossProduct(const Vector3& left, const Vector3& right);

		static Vector3 Up;
		static Vector3 Forward;
		static Vector3 Right;
		static Vector3 AxisX;
		static Vector3 AxisY;
		static Vector3 AxisZ;
		static Vector3 Zero;
		static Vector3 One;
	};
#pragma pack(pop)

	CORE_API Vector3 operator +(double left, const Vector3& right);
	CORE_API Vector3 operator -(double left, const Vector3& right);
	CORE_API Vector3 operator *(double left, const Vector3& right);
	CORE_API Vector3 operator /(double left, const Vector3& right);
}

#include "Vector3.inl"