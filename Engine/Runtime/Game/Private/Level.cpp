// Copyright 2020 Aumoa.lib. All right reserved.

#include "Level.h"

#include "Framework/Actor.h"

using namespace std;

Level::Level() : Super()
	, world(nullptr)
{

}

Level::~Level()
{

}

void Level::LoadLevel()
{

}

void Level::UnloadLevel()
{
	world = nullptr;
}