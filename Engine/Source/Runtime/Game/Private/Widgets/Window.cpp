// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/Window.h"
#include "Layout/LayoutImpl.h"
#include "Layout/ArrangedChildrens.h"
#include "Draw/SlateWindowElementList.h"
#include "Draw/PaintArgs.h"
#include "Layout/Geometry.h"

DEFINE_LOG_CATEGORY(LogSlateWindow);

SWindow::SWindow(const std::wstring& name) : Super(name)
	, _lastDeltaTime(std::chrono::milliseconds(0))
{
}

void SWindow::ExecuteTick(std::chrono::duration<float> deltaTime)
{
	Tick(*(_cachedRootGeometry = MakeRootGeometry()), _lastDeltaTime = deltaTime);
}

void SWindow::ExecutePaint(SSlateWindowElementList* drawElements) const
{
	ScopedPtr paintArgs = drawElements->NewObject<SPaintArgs>(this, _lastDeltaTime);
	Paint(paintArgs.Get(), *_cachedRootGeometry, Rect(Vector2::GetZero(), GetDesiredSize()), drawElements, 0, IsEnabled());
}

Geometry SWindow::MakeRootGeometry() const
{
	return Geometry::MakeRoot(GetDesiredSize(), SlateLayoutTransform());
}

void SWindow::SetWindowSize(const Vector2& localSize)
{
	_localSize = localSize;
}

void SWindow::AddWidgetToScreen(SWidget* widget)
{
	check(widget);
	_screenWidgets.emplace_back(widget);
	widget->SetOuter(this);
}

SWidget* SWindow::GetWidgetInScreen(const std::wstring& name) const
{
	for (size_t i = 0; i < _screenWidgets.size(); ++i)
	{
		if (_screenWidgets[i]->GetName() == name)
		{
			return _screenWidgets[i];
		}
	}

	return nullptr;
}

void SWindow::RemoveWidgetFromScreen(const std::wstring& name)
{
	for (auto it = _screenWidgets.begin(); it != _screenWidgets.end(); ++it)
	{
		if ((*it)->GetName() == name)
		{
			DestroySubobject(*it);
			_screenWidgets.erase(it);
			return;
		}
	}

	SE_LOG(LogSlateWindow, Verbose, L"Could not found widget name: \"{}\". Abort.", name);
}

Vector2 SWindow::GetDesiredSize() const
{
	return _localSize;
}

void SWindow::OnArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const
{
	for (auto it = _screenWidgets.begin(); it != _screenWidgets.end(); ++it)
	{
		ESlateVisibility childVisibility = (*it)->GetVisibility();
		if (arrangedChildrens->Accepts(childVisibility))
		{
			// Add the information about this child to the output list (ArrangedChildren)
			arrangedChildrens->AddWidget(childVisibility, allottedGeometry.MakeChild(
				// The child widget being arranged
				*it,
				// Child's local position (i.e. position within parent)
				Vector2(0, 0),
				// Child's size
				_localSize
			));
		}
	}
}