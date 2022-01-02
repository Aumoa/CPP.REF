// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Panel/CanvasPanel.h"

class STextBlock;

class EDITOR_API SLogConsole : public SCanvasPanel
{
	GENERATED_BODY(SLogConsole)

public:
	SLogConsole();
	virtual ~SLogConsole() noexcept override;

	virtual Vector2 GetDesiredSize() override;

public:
	BEGIN_SLATE_ATTRIBUTE
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

private:
	SPROPERTY(LogText)
	STextBlock* LogText = nullptr;
	std::list<std::wstring> Stream;

private:
	void UpdateLogText();
	void OnLogged(std::wstring_view Message);
};