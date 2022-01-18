// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace DotNETUtilities;

using System;

public class DynamicCompilationException : Exception
{
    public DynamicCompilationException(string InPath) : base($"An error occurred in compile C# code file. File: {InPath}")
    {
    }

    public DynamicCompilationException(string InPath, string InMessage) : base($"An error occurred in compile C# code file. File: {InPath}, Message: {InMessage}")
    {
    }
}