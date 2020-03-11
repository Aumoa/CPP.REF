#pragma once

namespace SC::Game
{
	/// <summary> 3���� ���� ������ ǥ���մϴ�. </summary>
	struct Ray
	{
		/// <summary> ������ ���� ��ġ�� �����մϴ�. </summary>
		Vector3 Origin;

		/// <summary> ������ ������ �����մϴ�. </summary>
		Vector3 Direction;

		/// <summary> ������ �ּ� ���̸� �����մϴ�. </summary>
		double MinDepth = 0;

		/// <summary> ������ �ִ� ���̸� �����մϴ�. </summary>
		double MaxDepth = 1000000000.0;

		/// <summary> <see cref="Ray"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
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