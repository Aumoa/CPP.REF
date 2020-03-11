#pragma once

namespace SC::Game
{
	/// <summary> �ִϸ��̼� �Ű������� ����ϴ� Ʈ���� �ڷ����� ǥ���մϴ�. </summary>
	struct Trigger
	{
	private:
		bool value = false;

	public:
		/// <summary> <see cref="Trigger"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Trigger( bool initialTrigger = false )
			: value( initialTrigger )
		{

		}

		/// <summary> ���� ���¸� ��������, ���°� Ȱ��ȭ ���¶�� ��Ȱ��ȭ ���·� �����մϴ�. </summary>
		bool Action()
		{
			auto ret = value;
			value = false;
			return ret;
		}

		/// <summary> (Visual Studio ����) ���� ���¸� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, IsActive );

		/// <summary> ���� ���¸� �����ɴϴ�. </summary>
		bool IsActive_get() const
		{
			return value;
		}

		/// <summary> ���� ���¸� �����մϴ�. </summary>
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