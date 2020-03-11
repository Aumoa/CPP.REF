#pragma once

namespace SC::Game
{
	class GameObjectEnumerator : virtual public Object, virtual public IEnumerator<RefPtr<GameObject>>
	{
	public:
		using _Myvector = std::vector<RefPtr<GameObject>>;
		using _Myiter = _Myvector::iterator;

	private:
		_Myiter beg;
		_Myiter end;
		_Myiter cur;
		bool start = false;

	public:
		GameObjectEnumerator( _Myiter beg, _Myiter end )
			: beg( beg )
			, end( end )
		{
			
		}

		bool MoveNext()
		{
			if ( start == false )
			{
				start = true;
				cur = beg;
			}
			else
			{
				++cur;
			}

			if ( cur == end )
			{
				return false;
			}
			return true;
		}

		void Reset()
		{
			start = false;
		}

		RefPtr<GameObject> Current_get() override
		{
			return *cur;
		}
	};
}