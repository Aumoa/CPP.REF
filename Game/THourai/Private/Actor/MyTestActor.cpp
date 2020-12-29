// Copyright 2020 Aumoa.lib. All right reserved.

#include "Actor/MyTestActor.h"

using namespace std;
using namespace std::chrono;

AMyTestActor::AMyTestActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

AMyTestActor::~AMyTestActor()
{

}

void AMyTestActor::Tick(duration<double> deltaTime)
{
	Super::Tick(deltaTime);

	TH_LOG(LogTH, Verbose, L"AMyTestActor::Tick");
}