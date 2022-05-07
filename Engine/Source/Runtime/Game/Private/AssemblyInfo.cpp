// Copyright 2020-2022 Aumoa.lib. All right reserved.

DEFINE_ASSEMBLY_INFO;

#include "GameEngine.h"
#include "GameInstance.h"
#include "GameModule.h"
#include "GameObject.h"

#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"

#include "Components/ActorComponent.h"
#include "Components/ComponentMobility.h"
#include "Components/ComponentTransformSpace.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"

#include "EngineSubsystems/GameEngineSubsystem.h"
#include "EngineSubsystems/GameInputSystem.h"
#include "EngineSubsystems/GameInstanceSubsystem.h"
#include "EngineSubsystems/GameLevelSystem.h"
#include "EngineSubsystems/GameModuleSystem.h"
#include "EngineSubsystems/GameRenderSystem.h"

#include "GameFramework/Actor.h"
#include "GameFramework/AIController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/LocalPlayer.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Player.h"
#include "GameFramework/PlayerController.h"

#include "Info/GameMode.h"
#include "Info/Info.h"

#include "Level/Level.h"
#include "Level/World.h"
#include "Level/WorldType.h"

#include "Ticking/TickingGroup.h"
#include "Ticking/TickTaskLevelManager.h"

GENERATE_BODY(libty::Game::SGameEngine);
GENERATE_BODY(libty::Game::SGameInstance);
GENERATE_BODY(libty::Game::SGameModule);
GENERATE_BODY(libty::Game::SGameObject);

GENERATE_BODY(libty::Game::SCameraComponent);
GENERATE_BODY(libty::Game::APlayerCameraManager);

GENERATE_BODY(libty::Game::SActorComponent);
GENERATE_BODY(libty::Game::EComponentMobility);
GENERATE_BODY(libty::Game::EComponentTransformSpace);
GENERATE_BODY(libty::Game::SPrimitiveComponent);
GENERATE_BODY(libty::Game::SSceneComponent);

GENERATE_BODY(libty::Game::SGameEngineSubsystem);
GENERATE_BODY(libty::Game::SGameInputSystem);
GENERATE_BODY(libty::Game::SGameInstanceSubsystem);
GENERATE_BODY(libty::Game::SGameLevelSystem);
GENERATE_BODY(libty::Game::SGameModuleSystem);
GENERATE_BODY(libty::Game::SGameRenderSystem);

GENERATE_BODY(libty::Game::AActor);
GENERATE_BODY(libty::Game::AAIController);
GENERATE_BODY(libty::Game::AController);
GENERATE_BODY(libty::Game::SLocalPlayer);
GENERATE_BODY(libty::Game::APawn);
GENERATE_BODY(libty::Game::SPlayer);
GENERATE_BODY(libty::Game::APlayerController);

GENERATE_BODY(libty::Game::AGameMode);
GENERATE_BODY(libty::Game::AInfo);

GENERATE_BODY(libty::Game::SLevel);
GENERATE_BODY(libty::Game::SWorld);
GENERATE_BODY(libty::Game::EWorldType);

GENERATE_BODY(libty::Game::ETickingGroup);
GENERATE_BODY(libty::Game::STickTaskLevelManager);