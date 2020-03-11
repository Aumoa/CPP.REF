#pragma once

#include "SC.h"
#include "SC.Threading.h"

#if defined( __SC_GAME_EXPORT_SYMBOL__ )
#define sc_game_export_object( type ) type
#define sc_game_export_float( type, alignOf ) type
#define sc_game_export_symbol __declspec( dllexport )
#else
#define sc_game_export_object( type ) object
#define sc_game_export_float( type, alignOf ) std::array<float, alignOf>
#define sc_game_export_symbol __declspec( dllimport )
#endif

#include <map>
#include <string>
#include <memory>
#include <vector>
#include <filesystem>
#include <array>
#include <variant>

namespace SC::Game
{
	struct IAsyncLoad;

	class UnhandledErrorDetectedEventArgs;
	class Application;
	class SceneManager;
	class Debug;
	class Assets;
	class AssetBundle;
	class Scene;
	class Mesh;
	class SkinnedMesh;
	class Texture2D;
	class RenderTexture2D;
	class Material;
	class AnimationClip;
	class AnimatorController;
	class AnimationState;
	class CameraClearMode;
	class CameraClearModeColor;
	class CameraClearModeSkybox;
	class GameObject;
	class Component;
	class Transform;
	class Camera;
	class MeshFilter;
	class MeshRenderer;
	class SkinnedMeshRenderer;
	class Light;
	class DirectionalLight;
	class Bone;
	class Animator;

	namespace UI
	{
		class DispatcherEventArgs;
		class Brush;
		class SolidColorBrush;
		class RevealBrush;
		class TextFormat;
		class DependencyObject;
		class Element;
		class Canvas;
		class Border;
		class Image;
		class TextBlock;
		class Button;
		class ProgressBar;
		class Panel;
		class FixedPanel;
		class RelativePanel;
		class ViewBase;
		class GridView;
		class Page;
		class NavigationEventArgs;
		namespace Shapes
		{
			class Shape;
			class Rectangle;
		}
	}

	class AppDuplicateException;

	namespace Details
	{
		class GameLogic;
		class CDeviceContext;
	}
}

#undef min
#undef max

/* Enum */
#include "SC.Game/KeyCode.h"
#include "SC.Game/RenderQueueLayer.h"
#include "SC.Game/UI.Anchor.h"
#include "SC.Game/UI.DispatcherEventType.h"
#include "SC.Game/UI.TextAlignment.h"
#include "SC.Game/UI.TextVerticalAlignment.h"
#include "SC.Game/UI.MouseButtonType.h"

/* Interface */
#include "SC.Game/IAsyncLoad.h"

/* Struct */
#include "SC.Game/AppConfiguration.h"
#include "SC.Game/Vector.h"
#include "SC.Game/Quaternion.h"
#include "SC.Game/Ray.h"
#include "SC.Game/Vertex.h"
#include "SC.Game/Time.h"
#include "SC.Game/Input.h"
#include "SC.Game/Keyframe.h"
#include "SC.Game/Keyframes.h"
#include "SC.Game/Trigger.h"
#include "SC.Game/AnimVar.h"
#include "SC.Game/AnimationTransitionCondition.h"
#include "SC.Game/UI.Thickness.h"
#include "SC.Game/UI.DependencyProperty.h"
#include "SC.Game/UI.MouseMoveEventArgs.h"
#include "SC.Game/UI.MouseClickEventArgs.h"
#include "SC.Game/UI.KeyboardEventArgs.h"

/* Class */
#include "SC.Game/UnhandledErrorDetectedEventArgs.h"
#include "SC.Game/Application.h"
#include "SC.Game/SceneManager.h"
#include "SC.Game/Debug.h"
#include "SC.Game/Assets.h"
#include "SC.Game/AssetBundle.h"
#include "SC.Game/Scene.h"
#include "SC.Game/Mesh.h"
#include "SC.Game/Texture2D.h"
#include "SC.Game/RenderTexture2D.h"
#include "SC.Game/Material.h"
#include "SC.Game/AnimationClip.h"
#include "SC.Game/AnimatorController.h"
#include "SC.Game/AnimationState.h"
#include "SC.Game/CameraClearMode.h"
#include "SC.Game/CameraClearModeColor.h"
#include "SC.Game/CameraClearModeSkybox.h"
#include "SC.Game/GameObject.h"
#include "SC.Game/Component.h"
#include "SC.Game/Transform.h"
#include "SC.Game/Camera.h"
#include "SC.Game/MeshFilter.h"
#include "SC.Game/MeshRenderer.h"
#include "SC.Game/SkinnedMeshRenderer.h"
#include "SC.Game/Light.h"
#include "SC.Game/DirectionalLight.h"
#include "SC.Game/Bone.h"
#include "SC.Game/Animator.h"
#include "SC.Game/UI.EventDispatcher.h"
#include "SC.Game/UI.DispatcherEventArgs.h"
#include "SC.Game/UI.Brush.h"
#include "SC.Game/UI.SolidColorBrush.h"
#include "SC.Game/UI.RevealBrush.h"
#include "SC.Game/UI.TextFormat.h"
#include "SC.Game/UI.DependencyObject.h"
#include "SC.Game/UI.Element.h"
#include "SC.Game/UI.Canvas.h"
#include "SC.Game/UI.Border.h"
#include "SC.Game/UI.Image.h"
#include "SC.Game/UI.TextBlock.h"
#include "SC.Game/UI.Button.h"
#include "SC.Game/UI.ProgressBar.h"
#include "SC.Game/UI.Panel.h"
#include "SC.Game/UI.FixedPanel.h"
#include "SC.Game/UI.RelativePanel.h"
#include "SC.Game/UI.ViewBase.h"
#include "SC.Game/UI.GridView.h"
#include "SC.Game/UI.Page.h"
#include "SC.Game/UI.NavigationEventArgs.h"
#include "SC.Game/UI.Shapes.Shape.h"
#include "SC.Game/UI.Shapes.Rectangle.h"

/* Exception */
#include "SC.Game/AppDuplicateException.h"