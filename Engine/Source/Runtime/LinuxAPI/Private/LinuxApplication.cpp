// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_LINUX

#include "LinuxApplication.h"
#include "LinuxWindow.h"
#include "IO/FileReference.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <vector>

namespace Ayla
{
    LinuxApplication::LinuxApplication()
    {
    }

    std::shared_ptr<GenericWindow> LinuxApplication::MakeWindow(const GenericWindowDefinition& winDef)
    {
        return std::make_shared<LinuxWindow>(winDef);
    }

    Vector2N LinuxApplication::GetScreenResolution()
    {
        Display* display = XOpenDisplay(nullptr);
        if (!display) return Vector2N(0, 0);
        int screen = DefaultScreen(display);
        int width = DisplayWidth(display, screen);
        int height = DisplayHeight(display, screen);
        XCloseDisplay(display);
        return Vector2N(width, height);
    }

    void LinuxApplication::PumpMessages(std::vector<GenericPlatformInputEvent>& outInputEvents)
    {
        Display* display = XOpenDisplay(nullptr);
        if (!display) return;

        while (XPending(display))
        {
            XEvent event;
            XNextEvent(display, &event);

            switch (event.type)
            {
            case MotionNotify:
            {
                GenericPlatformInputEvent& input = InputEvents.emplace_back();
                input.Idx = input.IDX_MouseMove;
                GenericPlatformInputMouseMoveEvent& mouseMove = input.MouseMove();
                mouseMove.Location = Vector2N(event.xmotion.x, event.xmotion.y);
                break;
            }
            case ButtonPress:
            case ButtonRelease:
            {
                GenericPlatformInputEvent& input = InputEvents.emplace_back();
                input.Idx = input.IDX_MouseButton;
                GenericPlatformInputMouseButtonEvent& mouseButton = input.MouseButton();
                mouseButton.ButtonType =
                    (event.xbutton.button == Button1) ? GenericPlatformInputMouseButtonType::Left :
                    (event.xbutton.button == Button2) ? GenericPlatformInputMouseButtonType::Middle :
                    (event.xbutton.button == Button3) ? GenericPlatformInputMouseButtonType::Right :
                    GenericPlatformInputMouseButtonType::Unknown;
                mouseButton.bUp = (event.type == ButtonRelease);
                mouseButton.Location = Vector2N(event.xbutton.x, event.xbutton.y);
                break;
            }
            case DestroyNotify:
                // 종료 처리 필요시 구현
                break;
            }
        }

        XCloseDisplay(display);

        std::swap(InputEvents, outInputEvents);
        InputEvents.clear();
    }

    DirectoryReference LinuxApplication::GetEngineDirectory() const
    {
        // /proc/self/exe를 통해 실행 파일 경로를 얻음
        char exePath[1024] = {0};
        ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
        if (len > 0)
        {
            FileReference exeFile(String(exePath, len));
            return exeFile.GetDirectory().GetParent().GetParent().GetParent().GetAbsolute();
        }
        return DirectoryReference();
    }
}

#endif
