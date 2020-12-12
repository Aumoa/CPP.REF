:: Copyright 2020 Aumoa.lib. All right reserved.

cd ThirdParty

cd assimp
rmdir /s /q BINARIES
call generate_projects.bat
cd ..

cd PhysX\physx\
rmdir /s /q compiler\vc16win64
call generate_projects.bat
cd ../..

:: Exit from ThirdParty
cd ..

call Build\Program\net5.0\GUIDRemover.exe "ThirdParty"

pause