@ECHO OFF

ECHO.
ECHO Generate Solution and Project Files...
ECHO.

PUSHD "%~dp0"
  dotnet "Engine\Binaries\DotNET\AylaBuildTool.dll" ProjectFiles
POPD