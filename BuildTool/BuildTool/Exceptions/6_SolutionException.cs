// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace BuildTool;

class SolutionException : BuildToolException
{
    public SolutionException(string Message) : base(6, Message)
    {
    }
}