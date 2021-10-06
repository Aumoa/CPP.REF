// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Draw/SlateDrawElement.h"
//#include "Draw/SlateFontElement.h"
#include "Layout/SlateLayoutTransform.h"
#include <variant>

interface IRHIBuffer;
interface IRHIDeviceContext;
class SWindow;

class GAME_API SSlateWindowElementList : implements SObject
{
	GENERATED_BODY(SSlateWindowElementList)

public:
	using GenericSlateElement = std::variant<SlateDrawElement>;

private:
	SWindow* _paintWindow = nullptr;
	std::vector<GenericSlateElement> _drawElements;
	IRHIBuffer* _dynamicElementBuffer = nullptr;

public:
	SSlateWindowElementList(SWindow* paintWindow);

	void SortByLayer();
	void Add(const SlateDrawElement& rhs);
	//void Add(const SlateFontElement& rhs);
	void Clear();

	template<class TShader>
	uint64 ApplyAndCreateBuffer(IRHIDeviceContext* deviceContext, TShader* shader)
	{
		std::vector drawElements = shader->MakeElements(_drawElements);
		return CreateBuffer(deviceContext, drawElements.data(), sizeof(typename decltype(drawElements)::value_type) * drawElements.size());
	}

	std::span<GenericSlateElement const> GetSpan() const;
	int32 Num() const;
	const SWindow* GetPaintWindow() const;

	template<class... TArgs>
	GenericSlateElement& Emplace(TArgs&&... args)
	{
		return _drawElements.emplace_back(std::forward<TArgs>(args)...);
	}

private:
	uint64 CreateBuffer(IRHIDeviceContext* deviceContext, const void* drawElements, size_t sizeInBytes);
};