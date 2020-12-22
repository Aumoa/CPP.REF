:: Copyright 2020 Aumoa.lib. All right reserved.

SET SOURCE_DIR=.
SET GENERATOR=Visual Studio 16 2019
SET BINARIES_DIR="./BINARIES/x64"
cmake CMakeLists.txt -G "%GENERATOR%" -S %SOURCE_DIR% -B %BINARIES_DIR%