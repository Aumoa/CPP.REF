#pragma once

#define _VISUAL_STUDIO_EDITOR_ 
#include "SC.h"
#include "SC.Threading.h"
#include "SC.Game.h"

using namespace SC;
using namespace SC::Game;

#define declare_element( className, varName ) RefPtr varName = new className( #varName )
#define vs_property_auto( className, varName )\
vs_property( className, varName );\
className varName ## _get();\
void varName ## _set( className value );

/* System Class */
#include "App.h"
#include "GameVar.h"


/* UI Element */
#include "GaugeBar.h"


/* GameObjects */
#include "MainCamera.h"
#include "PlayableCharacterBase.h"
#include "Mokou.h"
#include "Kaguya.h"
#include "Bee.h"
#include "Scene0_Terrain.h"


/* Components */
#include "Movement.h"
#include "MouseEventDispatcher.h"
#include "KeyboardMovement.h"
#include "CombatBase.h"
#include "AutoRotate.h"


/* UI Pages */
#include "BasePage.h"
#include "DemoPage.h"
#include "LoadingPage.h"
#include "OnlyScriptPage.h"
#include "CharacterPickPage.h"
#include "BaseCombatLayout.h"
#include "ComplexPage.h"
#include "UITestPage.h"


/* Game Scenes */
#include "DemoScene.h"
#include "Scene00.h"
#include "Scene01.h"
#include "Scene_UITest.h"