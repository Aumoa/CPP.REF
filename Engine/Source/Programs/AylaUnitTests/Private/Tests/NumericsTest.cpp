// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Tests/NumericsTest.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
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
			}
		};
	}
}
