@ECHO OFF

ECHO.
ECHO Generate Solution and Project Files...
ECHO.

CD %~dp0
SET ENGINE_ROOT=%cd%

PUSHD "%ENGINE_ROOT%"
  dotnet "%ENGINE_ROOT%\Binaries\BuildTool\BuildTool.dll" Engine.Solution.cs
POPD