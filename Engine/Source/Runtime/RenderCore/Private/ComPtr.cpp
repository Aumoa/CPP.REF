// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "ComPtr.h"

void HR(SLogCategory& category, HRESULT hr)
{
    if (FAILED(hr))
    {
        _com_error com_error(hr);
        std::wstring msg = com_error.ErrorMessage();
        LogSystem::Log(category, ELogVerbosity::Fatal, msg);
    }
}

void HR_E(SLogCategory& category, HRESULT hr)
{
    if (FAILED(hr))
    {
        _com_error com_error(hr);
        std::wstring msg = com_error.ErrorMessage();
        LogSystem::Log(category, ELogVerbosity::Error, msg);
    }
}