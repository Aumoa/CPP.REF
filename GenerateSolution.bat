@echo off

echo.
echo Generate Solution and Project Files...
echo.

pushd %~dp0
  pushd ThirdParty\BuildTool
    set BUILDTOOL_SOLUTION=%cd%\BuildTool.sln

    if not exist Build (
      call :buildProject
    )

    pushd Build
      if not exist Release (
        call :buildProject
      )

      pushd Release
        if not exist net6.0 (
          call :buildProject
        )

        pushd net6.0
          if not exist BuildTool.dll (
            call :buildProject
          )

          set BUILDTOOL=%cd%\BuildTool.dll
        popd
      popd
    popd
  popd

  if "%1"=="" (
    set VS_VERSION=VS2022
  ) else (
    set VS_VERSION=%1
  )

  dotnet %BUILDTOOL% --Solution Engine.Solution.cs --VSVersion %VS_VERSION%
popd

exit /b 0

:buildProject
  dotnet build %BUILDTOOL_SOLUTION% -c Release
  goto :EOF