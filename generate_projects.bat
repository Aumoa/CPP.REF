:: Copyright 2020-2021 Aumoa.lib. All right reserved.

@echo off

cd ThirdParty

echo -
echo ---------- cmake assimp ----------
echo -

cd assimp
if exist BINARIES rmdir /s /q BINARIES
call generate_projects.bat
cd ..

echo -
echo ---------- cmake PhysX ----------
echo -

cd PhysX\physx\
cd compiler
if exist vc16win64 rmdir /s /q vc16win64
cd ..
call generate_projects.bat
cd ../..

:: Exit from ThirdParty
cd ..

call Program\Binaries\GUIDRemover\net5.0\GUIDRemover.exe "ThirdParty"
call Program\Binaries\GUIDRemover\net5.0\GUIDRemover.exe "Engine"
call Program\Binaries\GUIDRemover\net5.0\GUIDRemover.exe "Game"

pause