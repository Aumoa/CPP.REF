// Copyright 2020-2021 Aumoa.lib. All right reserved.

/// <summary>
/// AutomationTool 프로그램에서 실행될 수 있는 하위 프로그램에 대한 공통 항목을 제공합니다.
/// </summary>
abstract class Subprogram
{
    protected Subprogram()
    {

    }

    /// <summary>
    /// 하위 프로그램을 실행합니다.
    /// </summary>
    public abstract void Run();
}