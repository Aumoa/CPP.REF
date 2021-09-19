// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/UserWidget.h"
#include "GameEngine.h"
#include "GameLauncherInstance.h"
#include "GameFramework/LocalPlayer.h"
#include "Widgets/Window.h"

SUserWidget::SUserWidget() : Super()
{
}

SWidget* SUserWidget::BuildSlateResources()
{
	return nullptr;
}

void SUserWidget::ReleaseSlateResources()
{
}

void SUserWidget::AddWidgetToScreen()
{
	RemoveFromParent();

	if (GGameInstance)
	{
		if (SLocalPlayer* localPlayer = GGameInstance->GetLocalPlayer(); localPlayer)
		{
			if (SWindow* mainWindow = localPlayer->GetSlateWindow(); mainWindow)
			{
				if (_buildedContent = this->BuildSlateResources(); _buildedContent)
				{
					mainWindow->AddWidgetToScreen(_buildedContent);
				}
			}
		}
	}
}

void SUserWidget::RemoveFromParent()
{
	if (GGameInstance && _buildedContent)
	{
		if (SLocalPlayer* localPlayer = GGameInstance->GetLocalPlayer(); localPlayer)
		{
			if (SWindow* mainWindow = localPlayer->GetSlateWindow(); mainWindow)
			{
				mainWindow->RemoveWidgetFromScreen(_buildedContent->GetName());
				ReleaseSlateResources();
				_buildedContent = nullptr;
			}
		}
	}
}