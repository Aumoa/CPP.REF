@ECHO OFF

dir "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0"
dir "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um"
dir "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64"
exit /b 1

ECHO Setup

cd "%~dp0"
dotnet publish -c Shipping -o Engine\Binaries\DotNET Engine\Source\Programs\AylaBuildTool || exit /b 1
dotnet Engine\Binaries\DotNET\AylaBuildTool.dll build --target "ShaderCompileWorker" --config Development || exit /b 1