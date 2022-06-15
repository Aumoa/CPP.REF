@ECHO OFF

IF "%1"=="" (
	ECHO Required argument not provided.
	EXIT /b 1
)

:: Move to script path.
CD %~dp0

:: Move to engine root.
CD ..\..
SET ENGINE_ROOT=%cd%

SET BUILD_DIR="Build\AnyCPU\%1\Release\net6.0"
IF NOT EXIST "%BUILD_DIR%" (
	ECHO Build directory "%BUILD_DIR%" does not exists.
	EXIT /b 1
)

PUSHD "%BUILD_DIR%"
	SET BUILD_DIR=%cd%
	ECHO Build directory: %BUILD_DIR%
POPD

IF NOT EXIST Binaries (
	MKDIR Binaries
)

IF NOT EXIST "Binaries\%1" (
	PUSHD Binaries
		MKDIR "%1"
	POPD
)

PUSHD "Binaries\%1"
	SET BINARIES_DIR=%cd%
	ECHO Binaries directory: %BINARIES_DIR%
POPD

ECHO Copy all binaries.
robocopy "%BUILD_DIR%" "%BINARIES_DIR%" *.dll *.pdb *.json /MIR

ECHO Done.