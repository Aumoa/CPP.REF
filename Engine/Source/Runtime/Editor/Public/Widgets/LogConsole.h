// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Panel/CanvasPanel.h"

class SScrollBox;
class STextBlock;
class STextBox;

class EDITOR_API SLogConsole : public SCanvasPanel
{
	GENERATED_BODY(SLogConsole)

public:
	SLogConsole();
	virtual ~SLogConsole() noexcept override;

protected:
	virtual Vector2 ComputeDesiredSize() override;

public:
	BEGIN_SLATE_ATTRIBUTE
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

private:
	SPROPERTY(ScrollBox)
	SScrollBox* ScrollBox = nullptr;
	SPROPERTY(LogText)
	STextBlock* LogText = nullptr;
	SPROPERTY(ConsoleInput)
	STextBox* ConsoleInput = nullptr;
	std::wstring Stream;

private:
	void UpdateLogText();
	void OnLogged(std::wstring_view Message);
	void OnConsoleCommitted(std::wstring_view ConsoleInput);
};