// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "THGameInstance.h"
#include "Levels/StartupLevel.h"
#include "Assets/StaticMesh.h"

DEFINE_GAME_MODULE(THGameInstance);

THGameInstance::THGameInstance() : Super()
{
	StartupLevel = SubclassOf<::StartupLevel>::StaticClass();

	//SoftObjectPtr<StaticMesh> loadStaticMesh(L"Chess/Arts/Bishop/Mesh/bishop.fbx");
	//StaticMesh* staticMesh = loadStaticMesh.LoadObject();
}