@ECHO OFF

ECHO.
ECHO Generate Solution and Project Files...
ECHO.

PUSHD "%~dp0"
  SET BUILDTOOL=Engine\Binaries\DotNET\AylaBuildTool.dll
  IF NOT EXIST "%BUILDTOOL%" (
    CALL Setup.bat
    IF NOT "%errorlevel%"=="0" (
      EXIT /b %errorlevel%
    )
  )

  dotnet "%BUILDTOOL%" ProjectFiles
POPD