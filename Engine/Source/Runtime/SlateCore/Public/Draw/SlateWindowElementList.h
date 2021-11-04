// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Draw/SlateDrawElement.h"

class SWindow;

class SLATECORE_API SlateWindowElementList
{
	using BytePadding_256 = std::array<uint8, 256>;

	std::vector<SlateDrawElement> Elements;
	std::vector<BytePadding_256> Payloads;
	std::vector<std::shared_ptr<SObject>> ReferencedObjects;
	size_t PayloadBufSeek = 0;

public:
	SWindow* const PaintWindow;

public:
	SlateWindowElementList(SWindow* InPaintWindow);

	void SortByLayer();
	SlateDrawElement& AddUninitialized();
	void AddReferencedObject(std::shared_ptr<SObject> LockObject);
	void Clear();

	const std::vector<SlateDrawElement>& GetElements() const;
	const uint8* GetPayloadBuffer() const;

	template<class T>
	T& CreateUninitializedPayload(size_t& SeekPointer)
	{
		constexpr size_t PayloadSize = sizeof(T);
		static_assert(PayloadSize <= 256, "The size of payload cannot over the 256 bytes.");

		size_t TotalPayloadBuf = Payloads.size() * 256;
		size_t Slack = TotalPayloadBuf - PayloadBufSeek;

		if (Slack < PayloadSize)
		{
			Payloads.emplace_back();
		}

		SeekPointer = PayloadBufSeek;
		uint8* SeekBuf = const_cast<uint8*>(GetPayloadBuffer()) + SeekPointer;
		PayloadBufSeek += PayloadSize;
		return reinterpret_cast<T&>(*SeekBuf);
	}
};