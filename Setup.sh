#!/bin/sh

echo Setup

dotnet build -c Shipping Engine/Source/Programs/AylaBuildTool
dotnet Engine/Source/Programs/AylaBuildTool.dll build --target "ShaderCompileWorker" --config Development