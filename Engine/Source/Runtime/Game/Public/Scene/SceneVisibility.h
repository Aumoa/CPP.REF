// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Container/BitArray.h"
#include <vector>

class SScene;
class SRHIResource;
class SRHIDeviceContext;
class SRHIShader;
struct MinimalViewInfo;

class GAME_API SSceneVisibility : implements SObject
{
	GENERATED_BODY(SSceneVisibility)

private:
	SScene* _scene = nullptr;
	SRHIResource* _viewBuffer = nullptr;
	size_t _viewBufCapa = 0;

	BitArray _visibilityBits;
	size_t _visibilityCnt = 0;

public:
	SSceneVisibility(SScene* owner);

	void CalcVisibility(const MinimalViewInfo& view);
	void SetupView(SRHIDeviceContext* dc, SRHIShader* shader, size_t idx);

	inline void ForEachVisibleItem(std::function<void(size_t, size_t)> body)
	{
		size_t viewIdx = 0;
		for (ConstBitIterator bitIt(_visibilityBits); bitIt; ++bitIt)
		{
			RelativeBitReference relativeRef = bitIt.GetIndex();
			if (_visibilityBits.AccessCorrespondingBit(relativeRef))
			{
				body(relativeRef.BitIndex, viewIdx++);
			}
		}
	}

private:
	void FrustumCull();
	void ReadyBuffer(size_t capa, bool bAllowShrink);
};