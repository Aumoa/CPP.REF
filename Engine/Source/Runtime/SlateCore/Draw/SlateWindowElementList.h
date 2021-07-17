// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "Draw/SlateDrawElement.h"
#include "Layout/SlateLayoutTransform.h"

class SWindow;

class SlateWindowElementList : virtual public Object
{
public:
	using Super = Object;

private:
	const SWindow* _paintWindow = nullptr;
	std::vector<SlateDrawElement> _drawElements;

public:
	SlateWindowElementList(const SWindow* paintWindow) : Super()
		, _paintWindow(paintWindow)
	{
	}

	void SortByLayer()
	{
		auto _Pred = [](const SlateDrawElement& lhs, const SlateDrawElement& rhs)
		{
			return lhs.Layer - rhs.Layer;
		};

		sort(_drawElements.begin(), _drawElements.end(), _Pred);
	}

	std::span<SlateDrawElement const> GetSpan() const
	{
		return _drawElements;
	}

	void Add(const SlateDrawElement& rhs)
	{
		_drawElements.emplace_back(rhs);
	}

	template<class... TArgs>
	SlateDrawElement& Emplace(TArgs&&... args)
	{
		return _drawElements.emplace_back(std::forward<TArgs>(args)...);
	}

	int32 Num() const
	{
		return (int32)_drawElements.size();
	}

	const SWindow* GetPaintWindow() const
	{
		return _paintWindow;
	}
};