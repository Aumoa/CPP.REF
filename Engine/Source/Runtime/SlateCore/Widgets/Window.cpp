// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Window.h"

using namespace std;
using namespace std::chrono;

SWindow::SWindow() : Super()
	, _lastDeltaTime(0ms)
{
}

void SWindow::ExecuteTick(duration<float> deltaTime)
{
	Tick(*(_cachedRootGeometry = MakeRootGeometry()), _lastDeltaTime = deltaTime);
}

void SWindow::ExecutePaint(SlateWindowElementList* drawElements) const
{
	ScopedPtr paintArgs = drawElements->CreateSubobject<PaintArgs>(this, _lastDeltaTime);
	Paint(paintArgs.Get(), *_cachedRootGeometry, Rect(Vector2::GetZero(), GetDesiredSize()), drawElements, 0, IsEnabled());
}

Geometry SWindow::MakeRootGeometry() const
{
	return Geometry::MakeRoot(Vector2(1, 1), SlateLayoutTransform());
}

void SWindow::SetWindowSize(const Vector2& localSize)
{
	_localSize = localSize;
}

Vector2 SWindow::GetDesiredSize() const
{
	return _localSize;
}

void SWindow::OnArrangeChildren(ArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const
{
}