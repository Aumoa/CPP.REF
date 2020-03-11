#pragma once

namespace Touhou
{
	class ComplexPage : public BasePage
	{
		RefPtr<UI::FixedPanel> fixedPanel;

	public:
		ComplexPage();

		void Add( RefPtr<UI::Element> page );
		void Remove( RefPtr<UI::Element> page );

	private:
		void InitializeComponents();
	};
}