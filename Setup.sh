#!/bin/sh

echo Setup

if [ "$(uname)" -eq "Linux" ]; then
    echo "Installing Linux dependencies..."
    sudo apt-get update
    sudo apt-get install -y \
        libx11-dev \
        xorg-dev \
        libxext-dev \
        libxrandr-dev \
        libxi-dev
fi

dotnet build -c Shipping Engine/Source/Programs/AylaBuildTool
dotnet Engine/Binaries/DotNET/AylaBuildTool.dll build --target "ShaderCompileWorker" --config Development