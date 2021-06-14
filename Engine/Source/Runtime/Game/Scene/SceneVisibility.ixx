// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:SceneVisibility;

import std.core;
import SC.Runtime.Core;
import SC.Runtime.RenderCore;
import :MinimalViewInfo;

export class Scene;
export class RHIDeviceContext;
export class RHIShader;

using namespace std;
using namespace std::chrono;

export class SceneVisibility : virtual public Object
{
public:
	using Super = Object;

private:
	Scene* _scene = nullptr;
	RHIResource* _viewBuffer = nullptr;
	size_t _viewBufCapa = 0;

	vector<int32> _visibilityBits;
	size_t _visibilityCnt = 0;

public:
	SceneVisibility(Scene* owner);

	void CalcVisibility(const MinimalViewInfo& view);
	void SetupView(RHIDeviceContext* dc, RHIShader* shader, size_t idx);

	inline void ForEachVisibleItem(function<void(size_t)> body)
	{
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
					body(idx);
				}
			}
		}
	}

private:
	void FrustumCull(vector<int32>& bits);
	void ReadyBuffer(size_t capa, bool bAllowShrink);
};