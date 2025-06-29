// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_LINUX

#include "LinuxWindow.h"

namespace Ayla
{
    LinuxWindow::LinuxWindow(const GenericWindowDefinition& winDef)
    {
        m_Window = XCreateSimpleWindow(m_Display, DefaultRootWindow(m_Display), 100, 100, 400, 300, 1, 0, 0);
        XMapWindow(m_Display, m_Window);
        XFlush(m_Display);

        int nX = (int)winDef.DesiredScreenPosition.X;
        if (nX == -1) nX = 100;
        int nY = (int)winDef.DesiredScreenPosition.Y;
        if (nY == -1) nY = 100;
        int nWidth = (int)winDef.DesiredScreenSize.X;
        if (nWidth == -1) nWidth = 400;
        int nHeight = (int)winDef.DesiredScreenSize.Y;
        if (nHeight == -1) nHeight = 300;

        m_CachedDefinition = winDef;

        m_Display = XOpenDisplay(nullptr);
        if (m_Display == nullptr)
        {
            return nullptr;
        }

        int screen = DefaultScreen(m_Display);
        Window root = RootWindow(m_Display, screen);

        m_Window = XCreateSimpleWindow(
            m_Display, root, nX, nY, nWidth, nHeight, 1,
            BlackPixel(m_Display, screen), WhitePixel(m_Display, screen)
        );

        // 기본 이벤트 마스크 등 추가 가능
        XSelectInput(m_Display, m_Window, ExposureMask | KeyPressMask | StructureNotifyMask);

        // 윈도우 이름 설정
        XStoreName(ptr->m_Display, ptr->m_Window, "LinuxWindow");

        // 바로 표시하지 않고, Show()에서 XMapWindow 호출
        XFlush(ptr->m_Display);

        return ptr;
    }

    GenericWindowDefinition LinuxWindow::GetDefinition() const
    {
        return m_CachedDefinition;
    }

    void* LinuxWindow::GetOSWindowHandle() const
    {
        return reinterpret_cast<void*>(m_Window);
    }

    void LinuxWindow::Show()
    {
        if (m_Display && m_Window)
        {
            XMapWindow(m_Display, m_Window);
            XFlush(m_Display);
        }
    }

    void LinuxWindow::Hide()
    {
        if (m_Display && m_Window)
        {
            XUnmapWindow(m_Display, m_Window);
            XFlush(m_Display);
        }
    }

    Vector2N LinuxWindow::GetSize() const
    {
        if (!m_Display || !m_Window)
        {
            return Vector2N(0, 0);
        }

        XWindowAttributes attr;
        XGetWindowAttributes(m_Display, m_Window, &attr);
        return Vector2N(attr.width, attr.height);
    }
}

#endif
