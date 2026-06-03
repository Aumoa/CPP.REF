#!/bin/sh

set -e

SCRIPT_DIR=$(CDPATH= cd "$(dirname "$0")" && pwd)
ENGINE_DIR="$SCRIPT_DIR/../Engine"
BUILDTOOL="$ENGINE_DIR/Binaries/DotNET/AylaBuildTool.dll"

dotnet build "$ENGINE_DIR/Source/Programs/AylaBuildTool/AylaBuildTool.csproj"
dotnet "$BUILDTOOL" generate --generator VisualStudioCode --project "$SCRIPT_DIR/SampleGame.aproject"
