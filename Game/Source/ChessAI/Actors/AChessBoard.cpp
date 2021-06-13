// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Game;
import SC.Runtime.Game.Shaders;
import SC.Game.ChessAI;

using namespace std;

AChessBoard::AChessBoard() : Super()
{
	StaticMeshComponent* sm = CreateSubobject<StaticMeshComponent>();
	SetRootComponent(sm);
	sm->SetStaticMesh(CreateSubobject<StaticMeshTest>(L"TEST_Triangle", GameEngine::GetEngine()->GetColorVertexFactory()));
}

AChessBoardProxy* AChessBoard::CreateProxy()
{
	return CreateSubobject<AChessBoardProxy>(this);
}