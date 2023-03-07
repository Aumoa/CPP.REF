// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EditorEngineLoop.h"
#include "GenericPlatform/GenericApplication.h"
#include "Widgets/SWindow.h"

constexpr LogCategory LogEditorLoop(TEXT("LogEditorLoop"));

NEditorEngineLoop::NEditorEngineLoop()
{
}

NEditorEngineLoop::~NEditorEngineLoop() noexcept
{
}

std::unique_ptr<NEngineLoop::NInitializeContext> NEditorEngineLoop::PreInit()
{
    Log::Info(LogEditorLoop, TEXT("Start pre-initialize engine."));

    // Show splash window.
    NGenericSplash::Show();

    // Create initialization context.
    auto Context = std::make_unique<NInitializeContext>();
    Context->GraphicsTask.emplace(TEXT("Graphics"), 100.0f);
    return Context;
}

void NEditorEngineLoop::PostInit(std::unique_ptr<NInitializeContext> InitContext)
{
    NGenericSplash::Hide();
    NBootstrapTask::Clear();

    auto* App = NGenericApplication::Get();
    NGenericWindowDefinition Def;
    Def.bPrimaryWindow = true;
    Def.bSystemMenu = true;
    Def.bThickframe = true;
    Def.bSizebox = true;
    Def.bCaption = true;
    std::shared_ptr NativeWindow = App->MakeWindow(Def);
    EditorWindow = SNew(SWindow);
    EditorWindow->AttachWindow(NativeWindow);

    NativeWindow->Show();
}