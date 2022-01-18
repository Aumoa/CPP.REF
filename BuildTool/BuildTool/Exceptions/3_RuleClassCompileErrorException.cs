// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace BuildTool;

class RuleClassCompileErrorException : BuildToolException
{
    public RuleClassCompileErrorException(string Message) : base(3, Message)
    {
    }
}