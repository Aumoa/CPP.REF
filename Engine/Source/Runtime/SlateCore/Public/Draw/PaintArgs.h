// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"

class SWidget;

class SLATECORE_API SPaintArgs : implements SObject
{
	GENERATED_BODY(SPaintArgs)

private:
	const SWidget* _parent = nullptr;
	std::chrono::duration<float> _deltaTime;

public:
	SPaintArgs(const SWidget* parentWidget, std::chrono::duration<float> deltaTime);

	SPaintArgs* WithNewParent(const SWidget* parentWidget) const;

	const SWidget* GetParentWidget() const { return _parent; }
	std::chrono::duration<float> GetDeltaTime() const { return _deltaTime; }
};