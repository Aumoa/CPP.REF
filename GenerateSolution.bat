@ECHO OFF

ECHO.
ECHO Generate Solution and Project Files...
ECHO.

CD %~dp0
SET ENGINE_ROOT=%cd%

IF "%VCPKG_DIR%"=="" (
  SET VCPKG_DIR=%ENGINE_ROOT%\ThirdParty\Source\vcpkg
)

IF NOT EXIST "%VCPKG_DIR%\vcpkg.exe" (
  IF NOT EXIST "%VCPKG_DIR%\bootstrap-vcpkg.bat" (
    IF NOT EXIST "%VCPKG_DIR%" (
      MKDIR %VCPKG_DIR%
    )

    PUSHD "%VCPKG_DIR%"
      git clone https://github.com/microsoft/vcpkg.git .
      IF NOT "%errorlevel%" == "0" (
        EXIT \b %errorlevel%
      )
    POPD
  )
  
  PUSHD "%VCPKG_DIR%"
    CALL .\bootstrap-vcpkg.bat
    IF NOT "%errorlevel%" == "0" (
      EXIT \b %errorlevel%
    )
  POPD
)

PUSHD %ENGINE_ROOT%  
  CALL "%VCPKG_DIR%\vcpkg.exe" integrate install
  IF NOT "%errorlevel%" == "0" (
    EXIT \b %errorlevel%
  )
POPD

PUSHD "%ENGINE_ROOT%"
  dotnet "%ENGINE_ROOT%\Binaries\BuildTool\BuildTool.dll" Engine.Solution.cs
POPD