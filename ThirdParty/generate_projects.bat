:: Copyright 2020 Aumoa.lib. All right reserved.

cd assimp
rmdir /s /q BINARIES
start generate_projects.bat
cd ..

cd PhysX\physx\
rmdir /s /q compiler\vc16win64
start generate_projects.bat
cd ..\..