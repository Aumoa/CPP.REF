// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:Ray;

import std.core;
import :Vector;
import :StringUtils;

using namespace std;

export
{
	/// <summary>
	/// Represents ray.
	/// </summary>
	template<size_t N>
	struct Ray
	{
		/// <summary>
		/// The origin location.
		/// </summary>
		Vector<N> Origin;

		/// <summary>
		/// The vector direction.
		/// </summary>
		Vector<N> Direction;

		/// <summary>
		/// The distance. Represents infinity if it is nullopt.
		/// </summary>
		optional<float> Distance = 0;

		/// <summary>
		/// Initialize new <see cref="Ray"/> instance.
		/// </summary>
		constexpr Ray()
		{
		}

		/// <summary>
		/// Initialize new <see cref="Ray"/> instance.
		/// </summary>
		/// <param name="origin"> The origin location. </param>
		/// <param name="direction"> The vector direction. </param>
		/// <param name="distance"> The distance. </param>
		constexpr Ray(const Vector<N>& origin, const Vector<N>& direction, optional<float> distance = nullopt)
			: Origin(origin)
			, Direction(direction)
			, Distance(distance)
		{
		}

		/// <summary>
		/// Initialize new <see cref="Ray"/> instance.
		/// </summary>
		/// <param name="ray"> The initial value. </param>
		constexpr Ray(const Ray& ray) : Origin(ray.Origin), Direction(ray.Direction), Distance(ray.Distance)
		{
		}

		/// <summary>
		/// Compare nearly equals to target ray with epsilon value.
		/// </summary>
		/// <param name="rhs"> The target ray. </param>
		/// <param name="epsilon"> The epsilon value. If different of two components is lower than this values, is nearly equals. </param>
		/// <returns> Indicate two rays is nearly equals. </returns>
		constexpr bool NearlyEquals(const Ray& rhs, float epsilon) const
		{
			return Origin.NearlyEquals(rhs.Origin, epsilon)
				&& Direction.NearlyEquals(rhs.Direction, epsilon)
				&& MathEx::Abs(Distance.value_or(-1.0f) - rhs.Distance.value_or(-1.0f)) <= epsilon;
		}

		/// <summary>
		/// Get simple string represents this ray value.
		/// </summary>
		/// <param name="formatArgs"> The formatting args that use to std::format. </param>
		/// <returns> The simple string value. </returns>
		wstring ToString(wstring_view formatArgs = L"") const
		{
			return format(L"Origin: {}, Direction: {}", Origin.ToString(formatArgs), Direction.ToString(formatArgs));
		}
	};
}