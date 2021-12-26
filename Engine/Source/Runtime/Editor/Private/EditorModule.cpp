// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EditorModule.h"
#include "EditorEngine.h"

GENERATE_BODY(SEditorModule);

SEditorModule::SEditorModule() : Super()
{
}

SGameEngine* SEditorModule::CreateGameEngine()
{
	return gcnew SEditorEngine();
}

DEFINE_GAME_MODULE(SEditorModule);