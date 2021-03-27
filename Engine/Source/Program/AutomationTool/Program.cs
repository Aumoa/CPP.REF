// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using System.Collections.Generic;
using System.Linq;

/// <summary>
/// AutomationTool의 응용 프로그램 진입점을 정의합니다.
/// </summary>
class Program
{
    /// <summary>
    /// 허용되는 프로그램 목록을 여기에 전달합니다.
    /// 프로그램에 첫 번째로 전달된 인자에서 결정됩니다.
    /// </summary>
    private static Dictionary<string, Func<string[], Subprogram>> Programs = new()
    {
        ["GenerateEngineSolution"] = (InArgs) => new GenerateEngineSolution(InArgs),
    };

    /// <summary>
    /// AutomationTool 프로그램을 시작합니다.
    /// </summary>
    /// <param name="InArgs"> 첫 번째 인수는 GenerateGameProject가 입력됩니다. </param>
    static void Main(string[] InArgs)
    {
        InArgs = new string[]
        {
            "GenerateEngineSolution",
            "..\\..\\"
        };

        Log.TraceLog($"Automation Tool Started with {string.Join(" ", InArgs)}");

        // AutomationTool을 실행하기 위한 충분한 인수가 전달되었는지 검사합니다.
        if (InArgs.Length < 1)
        {
            throw new ArgumentException("첫 번째 매개변수로 {0} 중 하나를 입력합니다.", string.Join(", ", Programs.Keys.ToList()));
        }

        // 선택에 맞는 프로그램을 실행합니다.
        if (Programs.TryGetValue(InArgs[0], out var MyTool))
        {
            Subprogram MySub = MyTool(InArgs);
            MySub.Run();
        }
        else
        {
            IList<string> ProgramsList = Programs.Keys.ToList();
            throw new ArgumentException($"AutomationTool 프로그램의 첫 번째 매개변수가 잘못되었습니다. 매개변수는 {string.Join(", ", ProgramsList)}만 허용됩니다.");
        }
    }
}