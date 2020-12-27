// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <compare>
#include "TRefPtr.h"

namespace SC::Runtime::Core::Numerics
{
	struct Vector3;

#pragma pack(push, 8)
	struct CORE_API Vector4
	{
		using This = Vector4;

		double X;
		double Y;
		double Z;
		double W;

		Vector4();
		Vector4(const Vector3& v3, double w);
		Vector4(double x, double y, double z, double w);
		Vector4(double splat);
		Vector4(const Vector4& copy);

		bool Equals(const Vector4& rh) const;
		bool NearlyEquals(const Vector4& rh, double epsilon) const;
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

		vs_property_get(double, LengthSq);
		double LengthSq_get() const;
		vs_property_get(double, Length);
		double Length_get() const;
		vs_property_get(Vector4, Normalized);
		Vector4 Normalized_get() const;
		vs_property(Vector4, Direction);
		Vector4 Direction_get() const;
		void Direction_set(const Vector4& value);

		const double& operator [](size_t index) const;
		double& operator [](size_t index);

		Vector4 operator -() const;
		Vector4 operator +(const Vector4& right) const;
		Vector4 operator -(const Vector4& right) const;
		Vector4 operator *(const Vector4& right) const;
		Vector4 operator /(const Vector4& right) const;
		double operator |(const Vector4& right) const;

		bool operator ==(const Vector4& right) const;
		bool operator !=(const Vector4& right) const;
		bool operator < (const Vector4& right) const;
		bool operator <=(const Vector4& right) const;
		bool operator > (const Vector4& right) const;
		bool operator >=(const Vector4& right) const;
		std::weak_ordering operator <=>(const Vector4& right) const;

		Vector4& operator +=(const Vector4& right);
		Vector4& operator -=(const Vector4& right);
		Vector4& operator *=(const Vector4& right);
		Vector4& operator /=(const Vector4& right);

		static double DistanceSq(const Vector4& left, const Vector4& right);
		static double Distance(const Vector4& left, const Vector4& right);
		static double DotProduct(const Vector4& left, const Vector4& right);
	};
#pragma pack(pop)

	CORE_API Vector4 operator +(double left, const Vector4& right);
	CORE_API Vector4 operator -(double left, const Vector4& right);
	CORE_API Vector4 operator *(double left, const Vector4& right);
	CORE_API Vector4 operator /(double left, const Vector4& right);
}

#include "Vector4.inl"