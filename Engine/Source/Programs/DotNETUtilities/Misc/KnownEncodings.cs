// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System.Text;

namespace AylaEngine;

public static class KnownEncodings
{
    static KnownEncodings()
    {
        Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
    }

    public static Encoding EUCKR => Encoding.GetEncoding(51949);
}
