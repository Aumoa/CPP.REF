// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Game;
import SC.Runtime.Game.Shaders;
import SC.Game.ChessAI;

using namespace std;
using namespace std::chrono;

AChessBoard::AChessBoard() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent* sm = CreateSubobject<StaticMeshComponent>();
	SetRootComponent(sm);
	sm->SetStaticMesh(CreateSubobject<StaticMeshTest>(L"TEST_Triangle", GameEngine::GetEngine()->GetColorVertexFactory()));
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