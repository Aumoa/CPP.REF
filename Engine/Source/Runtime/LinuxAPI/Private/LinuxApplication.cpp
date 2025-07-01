// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_LINUX

#include "LinuxApplication.h"
#include "LinuxWindow.h"
#include "IO/FileReference.h"
#include <unistd.h>
#include <vector>

namespace Ayla
{
    std::vector<GenericPlatformInputEvent> LinuxApplication::InputEvents;

    LinuxApplication::LinuxApplication()
        : m_Display{ XOpenDisplay(nullptr) }
    {
    }

    LinuxApplication::~LinuxApplication() noexcept
    {
        XCloseDisplay(m_Display);
        m_Display = nullptr;
    }

    std::shared_ptr<GenericWindow> LinuxApplication::MakeWindow(const GenericWindowDefinition& winDef)
    {
        auto window = std::make_shared<LinuxWindow>(m_Display, winDef);
        auto lock = std::unique_lock{ m_Spinlock };
        m_WeakWindows.try_emplace(window->GetOSWindowHandle(), window);
        return window;
    }

    Vector2N LinuxApplication::GetScreenResolution()
    {
        int screen = DefaultScreen(m_Display);
        int width = DisplayWidth(m_Display, screen);
        int height = DisplayHeight(m_Display, screen);
        return Vector2N(width, height);
    }

    void LinuxApplication::PumpMessages(std::vector<GenericPlatformInputEvent>& outInputEvents)
    {
        while (XPending(m_Display))
        {
            XEvent event;
            XNextEvent(m_Display, &event);

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
                    (GenericPlatformInputMouseButtonType)-1;
                mouseButton.bUp = (event.type == ButtonRelease);
                mouseButton.Location = Vector2N(event.xbutton.x, event.xbutton.y);
                break;
            }
            case ClientMessage:
            {
                auto* id = reinterpret_cast<void*>(event.xclient.window);
                auto lock = std::unique_lock{ m_Spinlock };
                auto it = m_WeakWindows.find(id);
                if (it == m_WeakWindows.end())
                {
                    continue;
                }

                auto targetWindow = it->second.lock();
                lock.unlock();

                targetWindow->OnDestroy();
            }
            case DestroyNotify:
                // 종료 처리 필요시 구현
                break;
            }
        }

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

DEFINE_CREATE_GENERIC_APPLICATION(::Ayla::LinuxApplication);

#endif
