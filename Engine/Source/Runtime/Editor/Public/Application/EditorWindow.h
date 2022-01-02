// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Application/Window.h"

class SViewport;
class SPanelWidget;

class EDITOR_API SEditorWindow : public SWindow
{
	GENERATED_BODY(SEditorWindow)

private:
	SPROPERTY(EditorLayout)
	SPanelWidget* EditorLayout = nullptr;

	SPROPERTY(EditorViewport)
	SViewport* EditorViewport = nullptr;

public:
	SEditorWindow();

	virtual void InitViewport();
	SViewport* GetEditorViewport();

	virtual Vector2 GetDesiredSize() override;

protected:
	virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;
};