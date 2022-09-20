// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IO/MouseState.h"
#include "IO/MousePositionMode.h"
#include "IO/MouseButton.h"
#include "IPlatformMouse.gen.h"

SINTERFACE()
interface CORE_API IPlatformMouse : virtual public Object
{
    GENERATED_BODY()

    virtual MouseState GetState() = 0;
    virtual void ResetScrollWheelValue() = 0;
    virtual void SetMode(EMousePositionMode InPositionMode) = 0;
    virtual bool IsConnected() = 0;
    virtual bool IsVisible() = 0;
    virtual void SetVisible(bool bNewVisibility) = 0;

    DECLARE_MULTICAST_DELEGATE(CursorDelegate, Vector2N);
    CursorDelegate CursorMoved;
    DECLARE_MULTICAST_DELEGATE(MouseButtonDelegate, Vector2N, EMouseButton);
    MouseButtonDelegate MouseButtonPressed;
    MouseButtonDelegate MouseButtonReleased;
    DECLARE_MULTICAST_DELEGATE(MouseWheelDelegate, int32);
    MouseWheelDelegate MouseWheelScrolled;
};