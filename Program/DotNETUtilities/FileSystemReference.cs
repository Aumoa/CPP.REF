// Copyright 2017 CLOVERGAMES Co., Ltd. All right reserved.

using System.ComponentModel;
using System.Diagnostics;

namespace DotNETUtilities;

/// <summary>
/// .NET 환경에서 지원되는 파일 시스템에서 특정 경로에 대한 레퍼런스를 지원합니다.
/// </summary>
public abstract class FileSystemReference
{
    /// <summary>
    /// 상대정 경로 및 전체 경로를 포함하여 프로그램에서 인식할 수 있는 경로로 레퍼런스를 생성합니다.
    /// </summary>
    /// <param name="InPath"> 프로그램이 지원하는 경로 유형을 전달합니다. </param>
    public FileSystemReference(string InPath)
    {
        FullPath = NormalizePathSeparator(Path.GetFullPath(InPath));
        Name = Path.GetFileNameWithoutExtension(FullPath);
        Root = Path.GetPathRoot(FullPath);
    }

    /// <summary>
    /// 대상 레퍼런스와 상대적 경로를 가져옵니다.
    /// </summary>
    /// <param name="InReference"> 대상 레퍼런스를 전달합니다. </param>
    /// <returns> 상대적 경로가 반환됩니다. </returns>
    public string GetRelativePath(FileSystemReference InReference)
    {
        return Path.GetRelativePath(FullPath, InReference.FullPath);
    }

    /// <summary>
    /// 상위 디렉토리를 가져옵니다.
    /// </summary>
    /// <returns> 디렉토리 레퍼런스가 반환됩니다. </returns>
    public DirectoryReference GetParent()
    {
        return new(Directory.GetParent(FullPath)?.FullName ?? "");
    }

    /// <summary>
    /// 레퍼런스 이름을 가져옵니다.
    /// </summary>
    /// <returns> 전체 경로가 반환됩니다. </returns>
    public override string ToString()
    {
        return FullPath;
    }

    /// <summary>
    /// 해당 경로를 파일 탐색기에서 엽니다.
    /// </summary>
    /// <param name="explorerName"> 탐색기에 해당하는 프로그램 이름을 전달합니다. </param>
    /// <returns> 성공 여부가 반환됩니다.</returns>
    public bool TryOpenExplorer(string explorerName = "explorer.exe")
    {
        try
        {
            OpenExplorer(explorerName);
            return true;
        }
        catch (Win32Exception)
        {
            return false;
        }
    }

    /// <summary>
    /// 해당 경로를 파일 탐색기에서 엽니다.
    /// </summary>
    /// <param name="explorerName"> 탐색기에 해당하는 프로그램 이름을 전달합니다. </param>
    public void OpenExplorer(string explorerName = "explorer.exe")
    {
        Process.Start(explorerName, FullPath);
    }

    /// <summary>
    /// 지정된 경로에서 대상의 이름을 가져옵니다.
    /// </summary>
    public string Name { get; }

    /// <summary>
    /// 전체 경로를 가져옵니다.
    /// </summary>
    public string FullPath { get; }

    /// <summary>
    /// 경로의 루트를 가져옵니다.
    /// </summary>
    public string? Root { get; }

    /// <summary>
    /// 경로의 구분자를 애플리케이션의 지원 구분자 형식으로 정규화합니다.
    /// </summary>
    /// <param name="inPath"> 경로를 전달합니다. </param>
    /// <returns> 정규화된 경로가 반환됩니다. </returns>
    protected static string NormalizePathSeparator(string inPath)
    {
        // Add separator pattern to this.
        return Path.DirectorySeparatorChar switch
        {
            // Pattern for Linux, etc...
            '/' => inPath.Replace('\\', Path.DirectorySeparatorChar),

            // Pattern for Windows, etc...
            '\\' => inPath.Replace('/', Path.DirectorySeparatorChar),

            _ => throw new NotImplementedException($"서브시스템에 대한 정규화 패턴이 지정되지 않았습니다. 경로 분리 지정자: {Path.DirectorySeparatorChar}")
        };
    }
}
