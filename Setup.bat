@ECHO OFF

ECHO Setup

cd "%~dp0"
dotnet publish -c Shipping -o Engine\Binaries\DotNET Engine\Source\Programs\AylaBuildTool || exit /b 1
dotnet Engine\Binaries\DotNET\AylaBuildTool.dll build --target "ShaderCompileWorker" --config Development || exit /b 1