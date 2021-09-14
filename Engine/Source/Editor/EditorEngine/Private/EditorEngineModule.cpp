// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "EditorEngineModule.h"
#include "EditorEngine.h"

SEditorEngineModule::SEditorEngineModule() : Super()
{
}

SEditorEngineModule::~SEditorEngineModule()
{
}

SGameEngine* SEditorEngineModule::CreateGameEngine()
{
	return NewObject<SEditorEngine>();
}

DEFINE_GAME_MODULE(SEditorEngineModule);