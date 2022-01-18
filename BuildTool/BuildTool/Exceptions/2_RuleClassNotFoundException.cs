// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace BuildTool;

class RuleClassNotFoundException : BuildToolException
{
    public RuleClassNotFoundException(string Message) : base(-2, Message)
    {
    }
}