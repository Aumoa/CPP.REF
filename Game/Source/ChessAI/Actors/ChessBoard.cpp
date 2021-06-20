// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessBoard.h"
#include "Components/StaticMeshComponent.h"
#include "Assets/AssetImporter.h"
#include "GameEngine.h"
#include "Pawns/ChessBoardProxy.h"
#include "Shaders/ColorShader/ColorVertexFactory.h"

using namespace std;
using namespace std::chrono;

AChessBoard::AChessBoard() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent* smc = CreateSubobject<StaticMeshComponent>();
	SetRootComponent(smc);

	AssetImporter* assimp = GameEngine::GetEngine()->GetAssetImporter();
	StaticMesh* sm = assimp->ImportStaticMesh(L"Contents/Chess/Arts/King/Mesh/king.fbx");
	smc->SetStaticMesh(sm);
}

void AChessBoard::TickActor(duration<float> elapsedTime, ActorTickFunction* tickFunction)
{
	Super::TickActor(elapsedTime, tickFunction);
	_rotation.Value += elapsedTime.count() * 90.0f;
	SetActorRotation(Quaternion::FromAxisAngle(Vector3(0, 1, 0), _rotation));
}

AChessBoardProxy* AChessBoard::CreateProxy()
{
	AChessBoardProxy* proxy = CreateSubobject<AChessBoardProxy>(this);
	proxy->SetBoard(this);
	return proxy;
}