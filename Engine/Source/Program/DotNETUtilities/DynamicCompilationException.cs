// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;

/// <summary>
/// 동적 컴파일 중 복구할 수 없는 오류가 발생하였을 때 발생합니다.
/// </summary>
public class DynamicCompilationException : Exception
{
    /// <summary>
    /// 파일 경로를 받아 기본적인 메시지를 사용합니다.
    /// </summary>
    /// <param name="InPath"> 파일 경로를 전달합니다. </param>
    public DynamicCompilationException(string InPath) : base($"코드를 컴파일하는 중 오류가 발생하였습니다. 파일 이름: {InPath}")
    {

    }

    /// <summary>
    /// 파일 경로와 구체적인 오류 메시지를 사용합니다.
    /// </summary>
    /// <param name="InPath"> 파일 경로를 전달합니다. </param>
    /// <param name="InMessage"> 구체적인 컴파일 메시지를 전달합니다. </param>
    public DynamicCompilationException(string InPath, string InMessage) : base($"코드를 컴파일하는 중 오류가 발생하였습니다. 파일 이름: {InPath}, 메시지: {InMessage}")
    {

    }
}