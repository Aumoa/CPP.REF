// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <compare>
#include "TRefPtr.h"

namespace SC::Runtime::Core
{
	class String;
}

namespace SC::Runtime::Core::Numerics
{
	struct Rectangle;

#pragma pack(push, 8)
	struct CORE_API Vector2
	{
		double X;
		double Y;

		Vector2();
		Vector2(double x, double y);
		Vector2(double splat);
		Vector2(const Vector2& copy);
		
		bool Equals(const Vector2& rh) const;
		bool NearlyEquals(const Vector2& rh, double epsilon) const;
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

		bool IsOverlap(const Rectangle& rect) const;

		vs_property_get(double, LengthSq);
		double LengthSq_get() const;
		vs_property_get(double, Length);
		double Length_get() const;
		vs_property_get(Vector2, Normalized);
		Vector2 Normalized_get() const;
		vs_property(Vector2, Direction);
		Vector2 Direction_get() const;
		void Direction_set(const Vector2& value);

		const double& operator [](size_t index) const;
		double& operator [](size_t index);

		Vector2 operator -() const;
		Vector2 operator +(const Vector2& right) const;
		Vector2 operator -(const Vector2& right) const;
		Vector2 operator *(const Vector2& right) const;
		Vector2 operator /(const Vector2& right) const;
		double operator |(const Vector2& right) const;

		bool operator ==(const Vector2& right) const;
		bool operator !=(const Vector2& right) const;
		bool operator < (const Vector2& right) const;
		bool operator <=(const Vector2& right) const;
		bool operator > (const Vector2& right) const;
		bool operator >=(const Vector2& right) const;
		std::weak_ordering operator <=>(const Vector2& right) const;

		Vector2& operator +=(const Vector2& right);
		Vector2& operator -=(const Vector2& right);
		Vector2& operator *=(const Vector2& right);
		Vector2& operator /=(const Vector2& right);

		static double DistanceSq(const Vector2& left, const Vector2& right);
		static double Distance(const Vector2& left, const Vector2& right);
		static double DotProduct(const Vector2& left, const Vector2& right);
	};
#pragma pack(pop)

	CORE_API Vector2 operator +(double left, const Vector2& right);
	CORE_API Vector2 operator -(double left, const Vector2& right);
	CORE_API Vector2 operator *(double left, const Vector2& right);
	CORE_API Vector2 operator /(double left, const Vector2& right);
}

#include "Vector2.inl"