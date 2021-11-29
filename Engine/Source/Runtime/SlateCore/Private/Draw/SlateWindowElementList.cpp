// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/SlateWindowElementList.h"

SlateWindowElementList::SlateWindowElementList(SWindow* InPaintWindow)
	: PaintWindow(InPaintWindow)
{
}

void SlateWindowElementList::SortByLayer()
{
	std::sort(Elements.begin(), Elements.end(), [](const auto& Lhs, const auto& Rhs)
	{
		return Lhs.Layer < Rhs.Layer;
	});
}

SlateDrawElement& SlateWindowElementList::AddUninitialized()
{
	return Elements.emplace_back();
}

void SlateWindowElementList::AddReferencedObject(std::shared_ptr<SObject> LockObject)
{
	ReferencedObjects.emplace_back(LockObject);
}

void SlateWindowElementList::Clear()
{
	Elements.clear();
	ReferencedObjects.clear();
	PayloadBufSeek = 0;
}

const std::vector<SlateDrawElement>& SlateWindowElementList::GetElements() const
{
	return Elements;
}

const uint8* SlateWindowElementList::GetPayloadBuffer() const
{
	return reinterpret_cast<const uint8*>(Payloads.data()->data());
}