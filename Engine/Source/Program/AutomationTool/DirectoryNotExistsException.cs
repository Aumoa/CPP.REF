// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;

/// <summary>
/// 필요한 디렉토리가 존재하지 않을 때 발생하는 예외입니다.
/// </summary>
class DirectoryNotExistsException : Exception
{
    /// <summary>
    /// 경로를 지정하여 예외를 생성합니다.
    /// </summary>
    /// <param name="DirectoryPath"> 디렉토리 경로를 전달합니다. </param>
    public DirectoryNotExistsException(string DirectoryPath) : base($"필요한 디렉토리({DirectoryPath})가 존재하지 않습니다.")
    {

    }
}