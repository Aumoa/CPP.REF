// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorWindow.h"

RefPtr<AEditorWindow> AEditorWindow::InternalGetWindow(TSubclassOf<AEditorWindow> WindowType)
{
	if (WindowType.GetType() == nullptr)
	{
		return nullptr;
	}

	RefPtr<AEditorWindow> Window = (AEditorWindow*)WindowType.GetType()->Construct();
	return Window;
}