// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System.Collections.Generic;
using System.IO;

/// <summary>
/// .NET 환경에서 지원되는 파일 시스템에서 특정 경로에 대한 디렉토리 레퍼런스를 지원합니다.
/// </summary>
public class DirectoryReference : FileSystemReference
{
    /// <summary>
    /// 상대정 경로 및 전체 경로를 포함하여 프로그램에서 인식할 수 있는 경로로 디렉토리 레퍼런스를 생성합니다.
    /// </summary>
    /// <param name="InPath"> 프로그램이 지원하는 경로 유형을 전달합니다. </param>
    public DirectoryReference(string InPath) : base(InPath)
    {

    }

    /// <summary>
    /// 이 레퍼런스에 해당하는 디렉토리가 존재하지 않는다면 새로 생성합니다.
    /// </summary>
    public void CreateIfNotExists()
    {
        if (!IsExist)
        {
            Directory.CreateDirectory(FullPath);
        }
    }

    /// <summary>
    /// 이 레퍼런스에 해당하는 디렉토리에서 특정 디렉토리로 이동합니다.
    /// </summary>
    /// <param name="ChangeDirectoryPath"> 이동하기 위한 상대 경로를 전달합니다. </param>
    /// <returns> 이동된 디렉토리 레퍼런스가 반환됩니다. </returns>
    public DirectoryReference Move(string ChangeDirectoryPath)
    {
        return new(Path.Combine(FullPath, ChangeDirectoryPath));
    }

    /// <summary>
    /// 현재 디렉토리에 있는 파일 레퍼런스를 가져옵니다.
    /// </summary>
    /// <returns> 파일 레퍼런스 목록을 가져옵니다. </returns>
    public FileReference[] GetCurrentFiles()
    {
        return ConvertFilePaths(Directory.GetFiles(FullPath, "*.*"));
    }

    /// <summary>
    /// 현재 디렉토리 및 하위 디렉토리에 있는 파일 레퍼런스를 가져옵니다.
    /// </summary>
    /// <returns> 파일 레퍼런스 목록을 가져옵니다. </returns>
    public FileReference[] GetAllFiles()
    {
        return ConvertFilePaths(Directory.GetFiles(FullPath, "*.*", SearchOption.AllDirectories));
    }

    /// <summary>
    /// 현재 디렉토리에 있는 하위 디렉토리 레퍼런스를 가져옵니다.
    /// </summary>
    /// <returns> 디렉토리 레퍼런스 목록을 가져옵니다. </returns>
    public DirectoryReference[] GetCurrentDirectories()
    {
        return ConvertDirectoryPaths(Directory.GetDirectories(FullPath, "*", SearchOption.TopDirectoryOnly));
    }

    /// <summary>
    /// 해당 레퍼런스에 해당하는 디렉토리이 존재하는지 검사합니다.
    /// </summary>
    public bool IsExist
    {
        get => Directory.Exists(FullPath);
    }

    /// <summary>
    /// 파일 경로를 파일 레퍼런스 형식으로 변환합니다.
    /// </summary>
    /// <param name="InPaths"> 경로 목록을 전달합니다. </param>
    /// <returns> 레퍼런스 목록을 반환합니다. </returns>
    private FileReference[] ConvertFilePaths(string[] InPaths)
    {
        if (InPaths is null || InPaths.Length == 0)
        {
            return new FileReference[0];
        }
        var References = new FileReference[InPaths.Length];
        for (int i = 0; i < InPaths.Length; ++i)
        {
            References[i] = new(InPaths[i]);
        }

        return References;
    }

    /// <summary>
    /// 디렉토리 경로를 디렉토리 레퍼런스 형식으로 변환합니다.
    /// </summary>
    /// <param name="InPaths"> 경로 목록을 전달합니다. </param>
    /// <returns> 레퍼런스 목록을 반환합니다. </returns>
    private DirectoryReference[] ConvertDirectoryPaths(string[] InPaths)
    {
        if (InPaths is null || InPaths.Length == 0)
        {
            return new DirectoryReference[0];
        }
        var References = new DirectoryReference[InPaths.Length];
        for (int i = 0; i < InPaths.Length; ++i)
        {
            References[i] = new(InPaths[i]);
        }

        return References;
    }
}