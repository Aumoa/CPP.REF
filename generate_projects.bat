:: Copyright 2020 Aumoa.lib. All right reserved.

@echo off

cd ThirdParty

cd assimp
if exist BINARIES rmdir /s /q BINARIES
call generate_projects.bat
cd ..

cd PhysX\physx\
cd compiler
if exist vc16win64 rmdir /s /q vc16win64
cd ..
call generate_projects.bat
cd ../..

:: Exit from ThirdParty
cd ..

call Build\Program\net5.0\GUIDRemover.exe "ThirdParty"
call Build\Program\net5.0\GUIDRemover.exe "Engine"
call Build\Program\net5.0\GUIDRemover.exe "Game"

pause