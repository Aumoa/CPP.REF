// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CompoundWidget.h"
#include "Layout/Geometry.h"

class GAME_API SWindow : public SCompoundWidget
{
	GENERATED_BODY(SWindow)

private:
	Vector2 _localSize;
	std::chrono::duration<float> _lastDeltaTime;
	std::optional<Geometry> _cachedRootGeometry;

	std::vector<SWidget*> _screenWidgets;

public:
	SWindow(const std::wstring& name);

	void ExecuteTick(std::chrono::duration<float> deltaTime);
	void ExecutePaint(SSlateWindowElementList* drawElements);
	Geometry MakeRootGeometry();

	void SetWindowSize(const Vector2& localSize);

	void AddWidgetToScreen(SWidget* widget);
	SWidget* GetWidgetInScreen(const std::wstring& name);
	void RemoveWidgetFromScreen(const std::wstring& name);

	virtual Vector2 GetDesiredSize();

protected:
	std::span<SWidget* const> GetWidgets() { return _screenWidgets; }

protected:
	virtual void OnArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) override;
};