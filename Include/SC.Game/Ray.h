#pragma once

namespace SC::Game
{
	/// <summary> 3차원 광선 정보를 표현합니다. </summary>
	struct Ray
	{
		/// <summary> 광선의 시작 위치를 설정합니다. </summary>
		Vector3 Origin;

		/// <summary> 광선의 방향을 설정합니다. </summary>
		Vector3 Direction;

		/// <summary> 광선의 최소 깊이를 설정합니다. </summary>
		double MinDepth = 0;

		/// <summary> 광선의 최대 깊이를 설정합니다. </summary>
		double MaxDepth = 1000000000.0;

		/// <summary> <see cref="Ray"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Ray() = default;

		constexpr bool operator==( const Ray& right ) const
		{
			return Origin == right.Origin && Direction == right.Direction && MinDepth == right.MinDepth && right.MaxDepth == right.MaxDepth;
		}

		constexpr bool operator!=( const Ray& right ) const
		{
			return Origin != right.Origin || Direction != right.Direction || MinDepth != right.MinDepth || right.MaxDepth != right.MaxDepth;
		}
	};
}

inline std::wostream& operator<<( std::wostream& wout, const SC::Game::Ray& ray )
{
	return wout << L"Ray(" << ray.Origin << L", " << ray.Direction << L", " << ray.MinDepth << L", " << ray.MaxDepth << L")";
}