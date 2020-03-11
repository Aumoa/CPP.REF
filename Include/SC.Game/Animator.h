#pragma once

namespace SC::Game
{
	/// <summary> 뼈대 정보가 존재하는 개체에 대해 애니메이션 갱신을 진행합니다. 이 클래스는 상속될 수 없습니다. </summary>
	class Animator final : public Component
	{
		struct tag_StateMachine
		{
			std::wstring Name;
			RefPtr<AnimationState> State;
			RefPtr<AnimationClip> Clip;
			double Duration = 0;
			std::map<std::string, Keyframes::KeyframePair, std::less<>> Keyframes;
			double TimePos = 0;

			void Interpolate( double timeDelta )
			{
				if ( Clip )
				{
					TimePos += timeDelta;
					Clip->Interpolate( TimePos, Keyframes );
				}
			}
		};

#pragma pack( push, 4 )
		struct tag_BoneTransform
		{
			sc_game_export_float( XMFLOAT4X4, 16 ) World;
		};

		struct tag_Bone
		{
			sc_game_export_float( XMFLOAT4X4, 16 ) Offset;
			sc_game_export_float( XMFLOAT4X4, 16 ) ToRoot;
		};
#pragma pack( pop )

		RefPtr<AnimatorController> controller;
		std::map<std::wstring, AnimVar, std::less<>> animVars;

		tag_StateMachine currentState;
		tag_StateMachine prevState;
		double blendTime = 0;

		std::vector<tag_Bone> boneTransform;
		std::array<sc_game_export_object( RefPtr<Details::CDynamicBuffer> ), 2> finalTransformBuffer;

	protected:
		void Render( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="Animator"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Animator();
		~Animator() override;

		/// <summary> (<see cref="ICloneable"/> 인터페이스에서 구현 됨.) 현재 인스턴스의 복사본인 새 개체를 생성합니다. </summary>
		object Clone() override;

		/// <summary> (<see cref="Component"/> 클래스에서 상속 됨.) 연결된 개체가 시작될 때 실행됩니다. </summary>
		void Start() override;

		/// <summary> (<see cref="Component"/> 클래스에서 상속 됨.) 컴포넌트에 대한 고정 프레임 논리 갱신을 수행합니다. </summary>
		/// <param name="time"> 게임 시간을 전달합니다. </param>
		void FixedUpdate( Time& time ) override;

		/// <summary> (Visual Studio 전용) <see cref="SC::Game::AnimatorController"/> 클래스에서 사용하는 애니메이션 매개변수의 값을 설정하거나 가져옵니다. </summary>
		vs_property( AnimVar, Vars )[];

		/// <summary> <see cref="AnimatorController"/> 클래스에서 사용하는 애니메이션 매개변수의 값을 가져옵니다. </summary>
		AnimVar Vars_get( const std::wstring_view& param0 );

		/// <summary> <see cref="AnimatorController"/> 클래스에서 사용하는 애니메이션 매개변수의 값을 설정합니다. </summary>
		void Vars_set( const std::wstring_view& param0, const AnimVar& value );

		/// <summary> (Visual Studio 전용) 애니메이션 컨트롤러 개체를 설정하거나 가져옵니다. </summary>
		vs_property( RefPtr<AnimatorController>, Controller );

		/// <summary> 재생할 애니메이션 클립을 가져옵니다. </summary>
		RefPtr<AnimatorController> Controller_get();

		/// <summary> 재생할 애니메이션 클립을 설정합니다. </summary>
		void Controller_set( RefPtr<AnimatorController> value );

	private:
		void InitializeOffset( RefPtr<GameObject> gameObject, int parent = -1 );

		void UpdateToRoot( RefPtr<GameObject> gameObject, int parent = -1 );
		void ReplaceToRoot();

		void Transistor();
		void UpdateBlend( double timeDelta );
		tag_StateMachine MakeStateMachine( const std::wstring_view& name );
	};
}