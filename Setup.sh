#!/bin/sh

echo Setup

dotnet build -c Shipping Engine/Source/Programs/AylaBuildTool
dotnet Engine/Binaries/DotNET/AylaBuildTool.dll build --target "ShaderCompileWorker" --config Development