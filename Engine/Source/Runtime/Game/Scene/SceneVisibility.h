// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include <vector>

class Scene;
class RHIResource;
class RHIDeviceContext;
class RHIShader;
struct MinimalViewInfo;

class SceneVisibility : virtual public Object
{
public:
	using Super = Object;

private:
	Scene* _scene = nullptr;
	RHIResource* _viewBuffer = nullptr;
	size_t _viewBufCapa = 0;

	BitArray _visibilityBits;
	size_t _visibilityCnt = 0;

public:
	SceneVisibility(Scene* owner);

	void CalcVisibility(const MinimalViewInfo& view);
	void SetupView(RHIDeviceContext* dc, RHIShader* shader, size_t idx);

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