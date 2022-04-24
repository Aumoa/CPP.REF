// Copyright 2020-2022 Aumoa.lib. All right reserved.

DEFINE_ASSEMBLY_INFO;

#include "WindowsApplication.h"

#include "Multimedia/WindowsImage.h"

#include "Input/WindowsIMEController.h"
#include "Input/WindowsPlatformKeyboard.h"
#include "Input/WindowsPlatformMouse.h"

GENERATE_BODY(libty::Windows::SWindowsApplication);

GENERATE_BODY(libty::Windows::SWindowsImage);

GENERATE_BODY(libty::Windows::SWindowsIMEController);
GENERATE_BODY(libty::Windows::SWindowsPlatformKeyboard);
GENERATE_BODY(libty::Windows::SWindowsPlatformMouse);