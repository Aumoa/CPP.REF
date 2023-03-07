// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineLoop.h"

class SWindow;

class NEditorEngineLoop : public NEngineLoop
{
private:
	std::shared_ptr<SWindow> EditorWindow;

public:
	NEditorEngineLoop();
	virtual ~NEditorEngineLoop() noexcept override;

	std::unique_ptr<NInitializeContext> PreInit();
	void PostInit(std::unique_ptr<NInitializeContext> InitContext);
};