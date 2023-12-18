// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "SubclassOf.h"
#include "EditorWindow.generated.h"

ACLASS()
class COREED_API AEditorWindow : public AObject
{
	GENERATED_BODY()

public:
	template<std::derived_from<AEditorWindow> TWindow>
	static RefPtr<TWindow> GetWindow()
	{
		return (RefPtr<TWindow>)InternalGetWindow(TWindow::StaticClass());
	}

private:
	static RefPtr<AEditorWindow> InternalGetWindow(TSubclassOf<AEditorWindow> WindowType);
};