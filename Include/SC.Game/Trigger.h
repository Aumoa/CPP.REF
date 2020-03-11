#pragma once

namespace SC::Game
{
	/// <summary> 애니메이션 매개변수로 사용하는 트리거 자료형을 표현합니다. </summary>
	struct Trigger
	{
	private:
		bool value = false;

	public:
		/// <summary> <see cref="Trigger"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Trigger( bool initialTrigger = false )
			: value( initialTrigger )
		{

		}

		/// <summary> 현재 상태를 가져오고, 상태가 활성화 상태라면 비활성화 상태로 변경합니다. </summary>
		bool Action()
		{
			auto ret = value;
			value = false;
			return ret;
		}

		/// <summary> (Visual Studio 전용) 현재 상태를 설정하거나 가져옵니다. </summary>
		vs_property( bool, IsActive );

		/// <summary> 현재 상태를 가져옵니다. </summary>
		bool IsActive_get() const
		{
			return value;
		}

		/// <summary> 현재 상태를 설정합니다. </summary>
		void IsActive_set( bool value )
		{
			this->value = value;
		}

		Trigger& operator=( const Trigger& right )
		{
			value = right.value;
			return *this;
		}

		bool operator==( const Trigger& right ) const
		{
			return value == right.value;
		}

		bool operator!=( const Trigger& right ) const
		{
			return value != right.value;
		}
	};
}

inline std::wostream& operator<<( std::wostream& wout, const SC::Game::Trigger& right )
{
	return wout << L"SC.Game.Trigger(" << right.IsActive_get() << L")";
}