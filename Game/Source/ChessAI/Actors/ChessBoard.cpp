// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessBoard.h"
#include "Components/StaticMeshComponent.h"
#include "Assets/AssetImporter.h"
#include "GameEngine.h"
#include "Pawns/ChessBoardProxy.h"
#include "Shaders/ColorShader/ColorVertexFactory.h"
#include "Shaders/ColorShader/ColorShader.h"
#include "Materials/MaterialInstance.h"

using namespace std;
using namespace std::chrono;

AChessBoard::AChessBoard() : Super()
{
	StaticMeshComponent* smc = CreateSubobject<StaticMeshComponent>();
	SetRootComponent(smc);

	AssetImporter* assimp = GameEngine::GetEngine()->GetAssetImporter();
	StaticMesh* sm = assimp->ImportStaticMesh(L"Contents/Chess/Arts/Board/Mesh/board.fbx");
	smc->SetStaticMesh(sm);

	ColorShader* cshader = GameEngine::GetEngine()->GetColorShader();
	MaterialInstance* black = CreateSubobject<MaterialInstance>(cshader->GetDefaultMaterial());
	black->SetScalarParameterValueByName(L"Color", 0.2f);
	smc->SetMaterial(0, black);

	MaterialInstance* white = CreateSubobject<MaterialInstance>(cshader->GetDefaultMaterial());
	white->SetScalarParameterValueByName(L"Color", 0.8f);
	smc->SetMaterial(1, white);

	StaticMeshComponent* king = CreateSubobject<StaticMeshComponent>();
	king->AttachToComponent(smc);

	sm = assimp->ImportStaticMesh(L"Contents/Chess/Arts/King/Mesh/king.fbx");
	king->SetStaticMesh(sm);
	king->SetMaterial(0, black);
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