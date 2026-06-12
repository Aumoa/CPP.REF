// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Tests/NumericsTest.h"
#include "Numerics/MatrixInterface/Matrix4x4.h"
#include "Numerics/TransformInterface/Quaternion.h"
#include "Numerics/VectorInterface/Rect.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	namespace
	{
		constexpr float TestEpsilon = 0.01f;

		void AssertNearlyEquals(const Vector2F& expected, const Vector2F& actual)
		{
			TestFixture::Assert::True(Vector<>::NearlyEquals(expected, actual, TestEpsilon));
		}

		void AssertNearlyEquals(const Vector3F& expected, const Vector3F& actual)
		{
			TestFixture::Assert::True(Vector<>::NearlyEquals(expected, actual, TestEpsilon));
		}

		void AssertNearlyEquals(const RectF& expected, const RectF& actual)
		{
			TestFixture::Assert::True(Rect<>::NearlyEquals(expected, actual, TestEpsilon));
		}
	}

	std::vector<TestFixture::TestCase> NumericsTest::SetUp()
	{
		return
		{
			TestCase
			{
				.Name = TEXT("Vector arithmetic"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						const Vector3F left(1.0f, 2.0f, 3.0f);
						const Vector3F right(4.0f, 5.0f, 6.0f);

						Assert::True(left + right == Vector3F(5.0f, 7.0f, 9.0f));
						Assert::True(right - left == Vector3F(3.0f, 3.0f, 3.0f));
						Assert::True(left * 2.0f == Vector3F(2.0f, 4.0f, 6.0f));
						Assert::Equal(32.0f, Vector<>::Dot(left, right));

						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Vector products"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						const Vector3F x(1.0f, 0.0f, 0.0f);
						const Vector3F y(0.0f, 1.0f, 0.0f);

						Assert::True(Vector<>::Cross(x, y) == Vector3F(0.0f, 0.0f, 1.0f));
						Assert::Equal(0.0f, Vector<>::Dot(x, y));
						Assert::Equal(1.0f, Vector<>::Length(x));

						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Safe normalization"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						Assert::True(Vector<>::SafeNormalize(Vector3F::Zero()) == Vector3F::Zero());
						const Vector3F normalized = Vector<>::SafeNormalize(Vector3F(0.0f, 3.0f, 4.0f));
						Assert::True(Math::Approximately(Vector<>::Length(normalized), 1.0f, 0.01f));

						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Matrix transforms"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						const auto translation = Matrix4x4<>::Translation(Vector3F(3.0f, 4.0f, 5.0f));
						AssertNearlyEquals(Vector3F(4.0f, 6.0f, 8.0f), translation.TransformPoint(Vector3F(1.0f, 2.0f, 3.0f)));
						AssertNearlyEquals(Vector3F(1.0f, 2.0f, 3.0f), translation.TransformVector(Vector3F(1.0f, 2.0f, 3.0f)));

						const auto scale = Matrix4x4<>::Scale(Scale3DF(2.0f, 3.0f, 4.0f));
						AssertNearlyEquals(Vector3F(2.0f, 6.0f, 12.0f), scale.TransformVector(Vector3F(1.0f, 2.0f, 3.0f)));
						Assert::True(Math::Approximately(Matrix4x4<>::Determinant(scale), 24.0f, TestEpsilon));

						const auto identity = Matrix4x4<>::Multiply(translation, translation.Inverse());
						Assert::True(Matrix<>::IsIdentity(identity, TestEpsilon));

						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Quaternion rotation"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						const auto rotation = QuaternionF::FromAxisAngle(Vector3F(0.0f, 0.0f, 1.0f), DegreesF(90.0f));
						const auto rotated = rotation.TransformVector(Vector3F(1.0f, 0.0f, 0.0f));
						AssertNearlyEquals(Vector3F(0.0f, 1.0f, 0.0f), rotated);

						const auto restored = rotation.Inverse().TransformVector(rotated);
						AssertNearlyEquals(Vector3F(1.0f, 0.0f, 0.0f), restored);
						Assert::True(rotation.Conjugate().NearlyEquals(rotation.Inverse(), TestEpsilon));

						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Rect operations"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						const RectF rect(0.0f, 0.0f, 10.0f, 20.0f);
						Assert::Equal(10.0f, Rect<>::Width(rect));
						Assert::Equal(20.0f, Rect<>::Height(rect));
						AssertNearlyEquals(Vector2F(5.0f, 10.0f), Rect<>::Center(rect));

						Assert::True(rect.Contains(Vector2F(5.0f, 10.0f)));
						Assert::False(rect.Contains(Vector2F(11.0f, 10.0f)));

						const auto intersection = Rect<>::Intersect(rect, RectF(5.0f, 8.0f, 12.0f, 30.0f));
						Assert::True(intersection.has_value());
						AssertNearlyEquals(RectF(5.0f, 8.0f, 10.0f, 20.0f), intersection.value());

						AssertNearlyEquals(RectF(-2.0f, -3.0f, 12.0f, 23.0f), rect.Extend(Vector2F(4.0f, 6.0f)));

						return Task<>::CompletedTask();
					}
				}
			}
		};
	}
}
