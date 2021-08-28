// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "Draw/SlateDrawElement.h"
#include "Layout/SlateLayoutTransform.h"

class GameEngine;
class SWindow;
class RHIDeviceContext;

class SLATECORE_API SlateWindowElementList : virtual public Object
{
	CLASS_BODY(SlateWindowElementList)

private:
	const SWindow* _paintWindow = nullptr;
	std::vector<SlateDrawElement> _drawElements;
	RHIResource* _dynamicElementBuffer = nullptr;

public:
	SlateWindowElementList(const SWindow* paintWindow);

	void SortByLayer();
	void Add(const SlateDrawElement& rhs);
	void Clear();

	template<class TShader>
	uint64 ApplyAndCreateBuffer(RHIDeviceContext* deviceContext, TShader* shader)
	{
		using ShaderDrawType = decltype(shader->MakeElement(std::declval<SlateRenderTransform>(), std::declval<Vector2>(), std::declval<float>()));
		std::vector<ShaderDrawType> drawElements(_drawElements.size());

		for (size_t i = 0; i < drawElements.size(); ++i)
		{
			const SlateDrawElement& element = _drawElements[i];

			if (element.Transform.HasRenderTransform())
			{
				drawElements[i] = shader->MakeElement(
					element.Transform.GetAccumulatedRenderTransform(),
					element.Transform.GetLocalSize(),
					(float)element.Layer
				);
			}
		}

		return CreateBuffer(deviceContext, drawElements.data(), sizeof(ShaderDrawType) * drawElements.size());
	}

	std::span<SlateDrawElement const> GetSpan() const;
	int32 Num() const;
	const SWindow* GetPaintWindow() const;

	template<class... TArgs>
	SlateDrawElement& Emplace(TArgs&&... args)
	{
		return _drawElements.emplace_back(std::forward<TArgs>(args)...);
	}

private:
	uint64 CreateBuffer(RHIDeviceContext* deviceContext, const void* drawElements, size_t sizeInBytes);
};