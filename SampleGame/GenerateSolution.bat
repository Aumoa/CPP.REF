@ECHO OFF
SETLOCAL

ECHO.
ECHO Generate Solution and Project Files...
ECHO.

PUSHD "%~dp0"
  SET "ENGINE_DIR=..\Engine"
  SET "BUILDTOOL=%ENGINE_DIR%\Binaries\DotNET\AylaBuildTool.dll"

  dotnet build "%ENGINE_DIR%\Source\Programs\AylaBuildTool\AylaBuildTool.csproj"
  IF ERRORLEVEL 1 GOTO :ExitWithError

  dotnet "%BUILDTOOL%" generate --generator VisualStudio --project ".\SampleGame.aproject"
  IF ERRORLEVEL 1 GOTO :ExitWithError
POPD
EXIT /B 0

:ExitWithError
SET "EXIT_CODE=%ERRORLEVEL%"
POPD
EXIT /B %EXIT_CODE%
