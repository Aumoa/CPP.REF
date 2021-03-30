// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;

/// <summary>
/// Build.cs 파일을 컴파일하는 중 오류가 발생하였을 때에 대한 예외입니다.
/// </summary>
class BuildFileCompileErrorException : Exception
{
    public BuildFileCompileErrorException(string Filename, string Message) : base($"빌드 파일(Build.cs)을 컴파일 하는 도중 오류가 발생하였습니다. 파일 이름: {Filename}, 메시지: {Message}")
    {

    }
}