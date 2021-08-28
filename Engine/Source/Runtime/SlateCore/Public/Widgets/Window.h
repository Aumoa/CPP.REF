// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "CompoundWidget.h"
#include "Layout/Geometry.h"

class SLATECORE_API SWindow : public SCompoundWidget
{
	CLASS_BODY(SWindow)

private:
	Vector2 _localSize;
	std::chrono::duration<float> _lastDeltaTime;
	std::optional<Geometry> _cachedRootGeometry;

public:
	SWindow();

	void ExecuteTick(std::chrono::duration<float> deltaTime);
	void ExecutePaint(SlateWindowElementList* drawElements) const;
	Geometry MakeRootGeometry() const;

	void SetWindowSize(const Vector2& localSize);

	virtual Vector2 GetDesiredSize() const;

protected:
	virtual void OnArrangeChildren(ArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const override;
};