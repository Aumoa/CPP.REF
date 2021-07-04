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

	std::vector<int32> _visibilityBits;
	size_t _visibilityCnt = 0;

public:
	SceneVisibility(Scene* owner);

	void CalcVisibility(const MinimalViewInfo& view);
	void SetupView(RHIDeviceContext* dc, RHIShader* shader, size_t idx);

	inline void ForEachVisibleItem(std::function<void(size_t, size_t)> body)
	{
		size_t viewIdx = 0;
		for (size_t i = 0; i < _visibilityBits.size(); ++i)
		{
			int32 n = _visibilityBits[i];
			int32 f = 1;
			for (size_t j = 0; j < 32; ++j)
			{
				size_t idx = i * 32 + j;

				const bool b = (n & f) != 0;
				f <<= 1;

				if (b)
				{
					body(idx, viewIdx++);
				}
			}
		}
	}

private:
	void FrustumCull(std::vector<int32>& bits);
	void ReadyBuffer(size_t capa, bool bAllowShrink);
};