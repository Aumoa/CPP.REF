// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace BuildTool;

class GeneratorException : BuildToolException
{
    public GeneratorException(string Message) : base(5, Message)
    {
    }
}