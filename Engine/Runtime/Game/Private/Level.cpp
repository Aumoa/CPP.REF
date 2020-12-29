// Copyright 2020 Aumoa.lib. All right reserved.

#include "Level.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;
using namespace SC::Runtime::Game::Framework;
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