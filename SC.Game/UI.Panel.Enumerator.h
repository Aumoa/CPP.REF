#pragma once

namespace SC::Game::UI
{
	class Panel::Enumerator : virtual public Object, virtual public IEnumerator<RefPtr<Element>>
	{
		using iterator = Panel::ContentType::element_type::iterator;
		iterator beginit;
		iterator endit;

		iterator current;
		bool started = false;

	public:
		Enumerator( iterator beginit, iterator endit );

		bool MoveNext() override;
		void Reset() override;
		RefPtr<Element> Current_get() override;
	};
}