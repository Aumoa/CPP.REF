// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_LINUX

#include "LinuxWindow.h"
#include "GenericPlatform/GenericApplication.h"

namespace Ayla
{
    LinuxWindow::LinuxWindow(Display* display, const GenericWindowDefinition& winDef)
        : m_Display{ display }
    {
        int nX = (int)winDef.DesiredScreenPosition.X;
        if (nX == -1) nX = 100;
        int nY = (int)winDef.DesiredScreenPosition.Y;
        if (nY == -1) nY = 100;
        int nWidth = (int)winDef.DesiredScreenSize.X;
        if (nWidth == -1) nWidth = 400;
        int nHeight = (int)winDef.DesiredScreenSize.Y;
        if (nHeight == -1) nHeight = 300;

        m_CachedDefinition = winDef;

        int screen = DefaultScreen(m_Display);
        Window root = RootWindow(m_Display, screen);

        m_Window = XCreateSimpleWindow(
            m_Display, root, nX, nY, nWidth, nHeight, 1,
            BlackPixel(m_Display, screen), WhitePixel(m_Display, screen)
        );

        Atom wmDelete = XInternAtom(m_Display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(m_Display, m_Window, &wmDelete, 1);

        XSelectInput(m_Display, m_Window, ExposureMask | KeyPressMask | StructureNotifyMask);
        XStoreName(m_Display, m_Window, "LinuxWindow");
        XFlush(m_Display);
    }

    LinuxWindow::~LinuxWindow() noexcept
    {
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

    void LinuxWindow::OnDestroy()
    {
        auto window = m_Window;
        m_Window = 0;
        XDestroyWindow(m_Display, window);

        if (m_CachedDefinition.bPrimaryWindow)
        {
            GenericApplication::Get().QuitApplication(0);
        }
    }
}

#endif
