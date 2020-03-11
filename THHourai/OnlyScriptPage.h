#pragma once

namespace Touhou
{
	class OnlyScriptPage : public BasePage
	{
		static RefPtr<UI::TextFormat> textFormatContinue;
		static RefPtr<UI::TextFormat> textFormatScript;

		const int numScripts;

		Diagnostics::StepTimer dropdownTick;
		bool dropdownState = true;

		std::vector<RefPtr<UI::TextBlock>> textSequences;
		std::vector<String> textScripts;
		RefPtr<UI::TextBlock> dropdownText;
		RefPtr<UI::TextBlock> helpText;

		RefPtr<UI::TextBlock> currentText;

		int currentSeqIndex = 0;

	public:
		OnlyScriptPage( int numScripts );

		void Update( Drawing::Rect<double> clientRect ) override;
		void ProcessEvent( RefPtr<UI::DispatcherEventArgs> args ) override;

		vs_property( String, Script )[];
		String Script_get( int param0 );
		void Script_set( int param0, String value );

		vs_property_set( String, HelpText );
		void HelpText_set( String value );

		Event<> ScriptEnded;

	private:
		void InitializeComponents();

		void MoveNext();
	};
}