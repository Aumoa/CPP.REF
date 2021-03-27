// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using System.IO;

/// <summary>
/// .NET 환경에서 지원되는 파일 시스템에서 특정 경로에 대한 파일 레퍼런스를 지원합니다.
/// </summary>
public class FileReference : FileSystemReference
{
    /// <summary>
    /// 상대정 경로 및 전체 경로를 포함하여 프로그램에서 인식할 수 있는 경로로 파일 레퍼런스를 생성합니다.
    /// </summary>
    /// <param name="InPath"> 프로그램이 지원하는 경로 유형을 전달합니다. </param>
    public FileReference(string InPath) : base(InPath)
    {
        Extension = Path.GetExtension(FullPath);

        if (Extension is not null)
        {
            FileName = $"{Name}{Extension}";
        }
        else
        {
            FileName = Name;
        }
    }

    /// <summary>
    /// 확장자를 비교합니다.
    /// </summary>
    /// <param name="InExtension"> 확장자를 전달합니다. </param>
    /// <returns> 비교 결과가 반환됩니다. </returns>
    public bool CompareExtension(string InExtension)
    {
        if (InExtension[0] is not '.')
        {
            InExtension = $".{InExtension}";
        }

        return Extension?.Equals(InExtension, StringComparison.OrdinalIgnoreCase) == true;
    }

    /// <summary>
    /// 해당 레퍼런스에 해당하는 파일이 존재하는지 검사합니다.
    /// </summary>
    public bool IsExist
    {
        get => File.Exists(FullPath);
    }

    /// <summary>
    /// 확장자가 포함된 파일 이름을 가져옵니다.
    /// </summary>
    public string FileName { get; }

    /// <summary>
    /// 지정된 경로에서 대상의 확장자를 가져옵니다.
    /// </summary>
    public string Extension { get; }

    /// <summary>
    /// 해당 레퍼런스에 해당하는 파일의 텍스트 데이터를 모두 읽습니다.
    /// </summary>
    /// <returns> 읽은 텍스트 데이터가 반환됩니다. </returns>
    public string ReaeAllText()
    {
        return File.ReadAllText(FullPath);
    }
}