// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/SlateVisibility.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SLATECORE_API SWidget
{
	SWidget(const SWidget&) = delete;
	SWidget(SWidget&&) = delete;

private:
	ESlateVisibility::Enum Visibility = ESlateVisibility::Visible;
	Vector2 DesiredSize;
	bool bEnabled : 1 = true;
	bool bInvalidated : 1 = true;

public:
	SWidget();
	virtual ~SWidget() noexcept;

	void SetVisibility(ESlateVisibility::Enum InVisibility);
	ESlateVisibility::Enum GetVisibility() const { return Visibility; }

	void SetEnabled(bool bInEnabled);
	bool IsEnabled() const { return bEnabled; }

	void Invalidate();
	bool IsInvalidated() const { return bInvalidated; }

protected:
	void CacheDesiredSize();
	virtual Vector2 ComputeDesiredSize() const = 0;

protected:
	virtual void OnVisibilityChanged([[maybe_unused]] ESlateVisibility::Enum PrevVisibility, [[maybe_unused]] ESlateVisibility::Enum NewVisibility) {}
	virtual void OnEnabled() {}
	virtual void OnDisabled() {}

private:
	void SetDesiredSize(const Vector2& InDesiredSize);
};